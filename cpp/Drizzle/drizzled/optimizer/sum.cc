/* - mode: c; c-basic-offset: 2; indent-tabs-mode: nil; -*-
 *  vim:expandtab:shiftwidth=2:tabstop=2:smarttab:
 *
 *  Copyright (C) 2008-2009 Sun Microsystems, Inc.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

/**
  @file

  Optimising of MIN(), MAX() and COUNT(*) queries without 'group by' clause
  by replacing the aggregate expression with a constant.

  Given a table with a compound key on columns (a,b,c), the following
  types of queries are optimised (assuming the table handler supports
  the required methods)

  @verbatim
  SELECT COUNT(*) FROM t1[,t2,t3,...]
  SELECT MIN(b) FROM t1 WHERE a=const
  SELECT MAX(c) FROM t1 WHERE a=const AND b=const
  SELECT MAX(b) FROM t1 WHERE a=const AND b<const
  SELECT MIN(b) FROM t1 WHERE a=const AND b>const
  SELECT MIN(b) FROM t1 WHERE a=const AND b BETWEEN const AND const
  SELECT MAX(b) FROM t1 WHERE a=const AND b BETWEEN const AND const
  @endverbatim

  Instead of '<' one can use '<=', '>', '>=' and '=' as well.
  Instead of 'a=const' the condition 'a IS NULL' can be used.

  If all selected fields are replaced then we will also remove all
  involved tables and return the answer without any join. Thus, the
  following query will be replaced with a row of two constants:
  @verbatim
  SELECT MAX(b), MIN(d) FROM t1,t2
    WHERE a=const AND b<const AND d>const
  @endverbatim
  (assuming a index for column d of table t2 is defined)
*/

#include <config.h>

#include <drizzled/sql_select.h>
#include <drizzled/item/sum.h>
#include <drizzled/item/cmpfunc.h>
#include <drizzled/optimizer/sum.h>
#include <drizzled/plugin/storage_engine.h>
#include <drizzled/table_list.h>
#include <drizzled/key.h>

namespace drizzled
{

static bool find_key_for_maxmin(bool max_fl,
                                table_reference_st *ref,
                                Field* field,
                                COND *cond,
                                uint32_t *range_fl,
                                uint32_t *key_prefix_length);

static int reckey_in_range(bool max_fl,
                           table_reference_st *ref,
                           Field* field,
                           COND *cond,
                           uint32_t range_fl,
                           uint32_t prefix_len);

static int maxmin_in_range(bool max_fl, Field *field, COND *cond);


/*
  Get exact count of rows in all tables

  SYNOPSIS
    get_exact_records()
    tables		List of tables

  NOTES
    When this is called, we know all table handlers supports HA_HAS_RECORDS
    or HA_STATS_RECORDS_IS_EXACT

  RETURN
    UINT64_MAX	Error: Could not calculate number of rows
    #			Multiplication of number of rows in all tables
*/
static uint64_t get_exact_record_count(TableList *tables)
{
  uint64_t count= 1;
  for (TableList *tl= tables; tl; tl= tl->next_leaf)
  {
    ha_rows tmp= tl->table->cursor->records();
    if ((tmp == HA_POS_ERROR))
    {
      return UINT64_MAX;
    }
    count*= tmp;
  }
  return count;
}


int optimizer::sum_query(TableList *tables, List<Item> &all_fields, COND *conds)
{
  List<Item>::iterator it(all_fields.begin());
  int const_result= 1;
  bool recalc_const_item= false;
  uint64_t count= 1;
  bool is_exact_count= true;
  bool maybe_exact_count= true;
  table_map removed_tables= 0;
  table_map outer_tables= 0;
  table_map used_tables= 0;
  table_map where_tables= 0;
  Item *item= NULL;
  int error;

  if (conds)
  {
    where_tables= conds->used_tables();
  }

  /*
     Analyze outer join dependencies, and, if possible, compute the number
     of returned rows.
   */
  for (TableList *tl= tables; tl; tl= tl->next_leaf)
  {
    TableList *embedded= NULL;
    for (embedded= tl; embedded; embedded= embedded->getEmbedding())
    {
      if (embedded->on_expr)
        break;
    }
    if (embedded)
      /* Don't replace expression on a table that is part of an outer join */
    {
      outer_tables|= tl->table->map;

      /*
         We can't optimise LEFT JOIN in cases where the WHERE condition
         restricts the table that is used, like in:
         SELECT MAX(t1.a) FROM t1 LEFT JOIN t2 join-condition
         WHERE t2.field IS NULL;
       */
      if (tl->table->map & where_tables)
        return 0;
    }
    else
    {
      used_tables|= tl->table->map;
    }

    /*
       If the storage manager of 'tl' gives exact row count as part of
       statistics (cheap), compute the total number of rows. If there are
       no outer table dependencies, this count may be used as the real count.
       Schema tables are filled after this function is invoked, so we can't
       get row count
     */
    if (! (tl->table->cursor->getEngine()->check_flag(HTON_BIT_STATS_RECORDS_IS_EXACT)))
    {
      maybe_exact_count&= test(tl->table->cursor->getEngine()->check_flag(HTON_BIT_HAS_RECORDS));
      is_exact_count= false;
      count= 1; // ensure count != 0
    }
    else
    {
      error= tl->table->cursor->info(HA_STATUS_VARIABLE | HA_STATUS_NO_LOCK);
      if(error)
      {
        tl->table->print_error(error, MYF(ME_FATALERROR));
        return error;
      }
      count*= tl->table->cursor->stats.records;
    }
  }

  /*
     Iterate through all items in the SELECT clause and replace
     COUNT(), MIN() and MAX() with constants (if possible).
   */

  while ((item= it++))
  {
    if (item->type() == Item::SUM_FUNC_ITEM)
    {
      Item_sum *item_sum= (((Item_sum*) item));
      switch (item_sum->sum_func())
      {
        case Item_sum::COUNT_FUNC:
          /*
             If the expr in COUNT(expr) can never be null we can change this
             to the number of rows in the tables if this number is exact and
             there are no outer joins.
           */
          if (! conds && ! ((Item_sum_count*) item)->args[0]->maybe_null &&
              ! outer_tables && maybe_exact_count)
          {
            if (! is_exact_count)
            {
              if ((count= get_exact_record_count(tables)) == UINT64_MAX)
              {
                /* Error from handler in counting rows. Don't optimize count() */
                const_result= 0;
                continue;
              }
              is_exact_count= 1;                  // count is now exact
            }
            ((Item_sum_count*) item)->make_const_count((int64_t) count);
            recalc_const_item= 1;
          }
          else
          {
            const_result= 0;
          }
          break;
        case Item_sum::MIN_FUNC:
          {
            /*
               If MIN(expr) is the first part of a key or if all previous
               parts of the key is found in the COND, then we can use
               indexes to find the key.
             */
            Item *expr=item_sum->args[0];
            if (expr->real_item()->type() == Item::FIELD_ITEM)
            {
              unsigned char key_buff[MAX_KEY_LENGTH];
              table_reference_st ref;
              uint32_t range_fl, prefix_len;

              ref.key_buff= key_buff;
              Item_field *item_field= (Item_field*) (expr->real_item());
              Table *table= item_field->field->getTable();

              /*
                 Look for a partial key that can be used for optimization.
                 If we succeed, ref.key_length will contain the length of
                 this key, while prefix_len will contain the length of
                 the beginning of this key without field used in MIN().
                 Type of range for the key part for this field will be
                 returned in range_fl.
               */
              if (table->cursor->inited ||
                  (outer_tables & table->map) ||
                  ! find_key_for_maxmin(0,
                                        &ref,
                                        item_field->field,
                                        conds,
                                        &range_fl,
                                        &prefix_len))
              {
                const_result= 0;
                break;
              }
              error= table->cursor->startIndexScan(static_cast<uint32_t>(ref.key), 1);
              if (error)
              {
                if (table->key_read)
                {
                  table->key_read= 0;
                  table->cursor->extra(HA_EXTRA_NO_KEYREAD);
                }
                table->print_error(error, MYF(0));
                return error;
              }

              if (! ref.key_length)
              {
                error= table->cursor->index_first(table->record[0]);
              }
              else
              {
                /*
                   Use index to replace MIN/MAX functions with their values
                   according to the following rules:

                   1) Insert the minimum non-null values where the WHERE clause still
                   matches, or
                   2) a NULL value if there are only NULL values for key_part_k.
                   3) Fail, producing a row of nulls

                   Implementation: Read the smallest value using the search key. If
                   the interval is open, read the next value after the search
                   key. If read fails, and we're looking for a MIN() value for a
                   nullable column, test if there is an exact match for the key.
                 */
                if (! (range_fl & NEAR_MIN))
                {
                  /*
                     Closed interval: Either The MIN argument is non-nullable, or
                     we have a >= predicate for the MIN argument.
                   */
                  error= table->cursor->index_read_map(table->record[0],
                                                       ref.key_buff,
                                                       make_prev_keypart_map(ref.key_parts),
                                                       HA_READ_KEY_OR_NEXT);
                }
                else
                {
                  /*
                     Open interval: There are two cases:
                     1) We have only MIN() and the argument column is nullable, or
                     2) there is a > predicate on it, nullability is irrelevant.
                     We need to scan the next bigger record first.
                   */
                  error= table->cursor->index_read_map(table->record[0],
                                                       ref.key_buff,
                                                       make_prev_keypart_map(ref.key_parts),
                                                       HA_READ_AFTER_KEY);
                  /*
                     If the found record is outside the group formed by the search
                     prefix, or there is no such record at all, check if all
                     records in that group have NULL in the MIN argument
                     column. If that is the case return that NULL.

                     Check if case 1 from above holds. If it does, we should read
                     the skipped tuple.
                   */
                  if (item_field->field->real_maybe_null() &&
                      ref.key_buff[prefix_len] == 1 &&
                      /*
                         Last keypart (i.e. the argument to MIN) is set to NULL by
                         find_key_for_maxmin only if all other keyparts are bound
                         to constants in a conjunction of equalities. Hence, we
                         can detect this by checking only if the last keypart is
                         NULL.
                       */
                      (error == HA_ERR_KEY_NOT_FOUND ||
                       key_cmp_if_same(table, ref.key_buff, ref.key, prefix_len)))
                  {
                    assert(item_field->field->real_maybe_null());
                    error= table->cursor->index_read_map(table->record[0],
                                                         ref.key_buff,
                                                         make_prev_keypart_map(ref.key_parts),
                                                         HA_READ_KEY_EXACT);
                  }
                }
              }
              /* Verify that the read tuple indeed matches the search key */
              if (! error &&
                  reckey_in_range(0,
                                  &ref,
                                  item_field->field,
                                  conds,
                                  range_fl,
                                  prefix_len))
              {
                error= HA_ERR_KEY_NOT_FOUND;
              }
              if (table->key_read)
              {
                table->key_read= 0;
                table->cursor->extra(HA_EXTRA_NO_KEYREAD);
              }
              table->cursor->endIndexScan();
              if (error)
              {
                if (error == HA_ERR_KEY_NOT_FOUND || error == HA_ERR_END_OF_FILE)
                {
                  return HA_ERR_KEY_NOT_FOUND;	      // No rows matching WHERE
                }
                /* HA_ERR_LOCK_DEADLOCK or some other error */
                table->print_error(error, MYF(0));
                return error;
              }
              removed_tables|= table->map;
            }
            else if (! expr->const_item() || ! is_exact_count)
            {
              /*
                 The optimization is not applicable in both cases:
                 (a) 'expr' is a non-constant expression. Then we can't
                 replace 'expr' by a constant.
                 (b) 'expr' is a costant. According to ANSI, MIN/MAX must return
                 NULL if the query does not return any rows. Thus, if we are not
                 able to determine if the query returns any rows, we can't apply
                 the optimization and replace MIN/MAX with a constant.
               */
              const_result= 0;
              break;
            }
            if (! count)
            {
              /* If count == 0, then we know that is_exact_count == true. */
              ((Item_sum_min*) item_sum)->clear(); /* Set to NULL. */
            }
            else
            {
              ((Item_sum_min*) item_sum)->reset(); /* Set to the constant value. */
            }
            ((Item_sum_min*) item_sum)->make_const();
            recalc_const_item= 1;
            break;
          }
        case Item_sum::MAX_FUNC:
          {
            /*
               If MAX(expr) is the first part of a key or if all previous
               parts of the key is found in the COND, then we can use
               indexes to find the key.
             */
            Item *expr= item_sum->args[0];
            if (expr->real_item()->type() == Item::FIELD_ITEM)
            {
              unsigned char key_buff[MAX_KEY_LENGTH];
              table_reference_st ref;
              uint32_t range_fl, prefix_len;

              ref.key_buff= key_buff;
              Item_field *item_field= (Item_field*) (expr->real_item());
              Table *table= item_field->field->getTable();

              /*
                 Look for a partial key that can be used for optimization.
                 If we succeed, ref.key_length will contain the length of
                 this key, while prefix_len will contain the length of
                 the beginning of this key without field used in MAX().
                 Type of range for the key part for this field will be
                 returned in range_fl.
               */
              if (table->cursor->inited ||
                  (outer_tables & table->map) ||
                  ! find_key_for_maxmin(1,
                                        &ref,
                                        item_field->field,
                                        conds,
                                        &range_fl,
                                        &prefix_len))
              {
                const_result= 0;
                break;
              }
              error= table->cursor->startIndexScan(static_cast<uint32_t>(ref.key), 1);

              if (! ref.key_length)
              {
                error= table->cursor->index_last(table->record[0]);
              }
              else
              {
                error= table->cursor->index_read_map(table->record[0],
                                                     key_buff,
                                                     make_prev_keypart_map(ref.key_parts),
                                                     range_fl & NEAR_MAX ?
                                                     HA_READ_BEFORE_KEY :
                                                     HA_READ_PREFIX_LAST_OR_PREV);
              }
              if (! error &&
                  reckey_in_range(1,
                                  &ref,
                                  item_field->field,
                                  conds,
                                  range_fl,
                                  prefix_len))
              {
                error= HA_ERR_KEY_NOT_FOUND;
              }
              if (table->key_read)
              {
                table->key_read= 0;
                table->cursor->extra(HA_EXTRA_NO_KEYREAD);
              }
              table->cursor->endIndexScan();
              if (error)
              {
                if (error == HA_ERR_KEY_NOT_FOUND || error == HA_ERR_END_OF_FILE)
                {
                  return HA_ERR_KEY_NOT_FOUND;	     // No rows matching WHERE
                }
                /* HA_ERR_LOCK_DEADLOCK or some other error */
                table->print_error(error, MYF(ME_FATALERROR));
                return error;
              }
              removed_tables|= table->map;
            }
            else if (! expr->const_item() || ! is_exact_count)
            {
              /*
                 The optimization is not applicable in both cases:
                 (a) 'expr' is a non-constant expression. Then we can't
                 replace 'expr' by a constant.
                 (b) 'expr' is a costant. According to ANSI, MIN/MAX must return
                 NULL if the query does not return any rows. Thus, if we are not
                 able to determine if the query returns any rows, we can't apply
                 the optimization and replace MIN/MAX with a constant.
               */
              const_result= 0;
              break;
            }
            if (! count)
            {
              /* If count != 1, then we know that is_exact_count == true. */
              ((Item_sum_max*) item_sum)->clear(); /* Set to NULL. */
            }
            else
            {
              ((Item_sum_max*) item_sum)->reset(); /* Set to the constant value. */
            }
            ((Item_sum_max*) item_sum)->make_const();
            recalc_const_item= 1;
            break;
          }
        default:
          const_result= 0;
          break;
      }
    }
    else if (const_result)
    {
      if (recalc_const_item)
      {
        item->update_used_tables();
      }
      if (! item->const_item())
      {
        const_result= 0;
      }
    }
  }
  /*
     If we have a where clause, we can only ignore searching in the
     tables if MIN/MAX optimisation replaced all used tables
     We do not use replaced values in case of:
     SELECT MIN(key) FROM table_1, empty_table
     removed_tables is != 0 if we have used MIN() or MAX().
   */
  if (removed_tables && used_tables != removed_tables)
  {
    const_result= 0;                            // We didn't remove all tables
  }
  return const_result;
}


bool optimizer::simple_pred(Item_func *func_item, Item **args, bool &inv_order)
{
  Item *item= NULL;
  inv_order= false;
  switch (func_item->argument_count())
  {
  case 0:
    /* MULT_EQUAL_FUNC */
    {
      Item_equal *item_equal= (Item_equal *) func_item;
      Item_equal_iterator it(item_equal->begin());
      args[0]= it++;
      if (it++)
      {
        return 0;
      }
      if (! (args[1]= item_equal->get_const()))
      {
        return 0;
      }
    }
    break;
  case 1:
    /* field IS NULL */
    item= func_item->arguments()[0];
    if (item->type() != Item::FIELD_ITEM)
    {
      return 0;
    }
    args[0]= item;
    break;
  case 2:
    /* 'field op const' or 'const op field' */
    item= func_item->arguments()[0];
    if (item->type() == Item::FIELD_ITEM)
    {
      args[0]= item;
      item= func_item->arguments()[1];
      if (! item->const_item())
      {
        return 0;
      }
      args[1]= item;
    }
    else if (item->const_item())
    {
      args[1]= item;
      item= func_item->arguments()[1];
      if (item->type() != Item::FIELD_ITEM)
      {
        return 0;
      }
      args[0]= item;
      inv_order= true;
    }
    else
    {
      return 0;
    }
    break;
  case 3:
    /* field BETWEEN const AND const */
    item= func_item->arguments()[0];
    if (item->type() == Item::FIELD_ITEM)
    {
      args[0]= item;
      for (int i= 1 ; i <= 2; i++)
      {
        item= func_item->arguments()[i];
        if (! item->const_item())
        {
          return 0;
        }
        args[i]= item;
      }
    }
    else
    {
      return 0;
    }
  }
  return 1;
}


/**
  Check whether a condition matches a key to get {MAX|MIN}(field):.

     For the index specified by the keyinfo parameter, index that
     contains field as its component (field_part), the function
     checks whether the condition cond is a conjunction and all its
     conjuncts referring to the columns of the same table as column
     field are one of the following forms:
     - f_i= const_i or const_i= f_i or f_i is null,
     where f_i is part of the index
     - field {<|<=|>=|>|=} const or const {<|<=|>=|>|=} field
     - field between const1 and const2

  @param[in]     max_fl         Set to 1 if we are optimising MAX()
  @param[in,out] ref            Reference to the structure we store the key
    value
  @param[in]     keyinfo        Reference to the key info
  @param[in]     field_part     Pointer to the key part for the field
  @param[in]     cond           WHERE condition
  @param[in,out] key_part_used  Map of matchings parts
  @param[in,out] range_fl       Says whether including key will be used
  @param[out]    prefix_len     Length of common key part for the range
    where MAX/MIN is searched for

  @retval
    0        Index can't be used.
  @retval
    1        We can use index to get MIN/MAX value
*/
static bool matching_cond(bool max_fl,
                          table_reference_st *ref,
                          KeyInfo *keyinfo,
                          KeyPartInfo *field_part,
                          COND *cond,
                          key_part_map *key_part_used,
                          uint32_t *range_fl,
                          uint32_t *prefix_len)
{
  if (! cond)
  {
    return 1;
  }
  Field *field= field_part->field;

  field->setWriteSet();

  if (! (cond->used_tables() & field->getTable()->map))
  {
    /* Condition doesn't restrict the used table */
    return 1;
  }
  if (cond->type() == Item::COND_ITEM)
  {
    if (((Item_cond*) cond)->functype() == Item_func::COND_OR_FUNC)
    {
      return 0;
    }

    /* AND */
    List<Item>::iterator li(((Item_cond*) cond)->argument_list()->begin());
    Item *item;
    while ((item= li++))
    {
      if (! matching_cond(max_fl,
                          ref,
                          keyinfo,
                          field_part,
                          item,
                          key_part_used,
                          range_fl,
                          prefix_len))
      {
        return 0;
      }
    }
    return 1;
  }

  if (cond->type() != Item::FUNC_ITEM)
  {
    return 0; // Not operator, can't optimize
  }

  bool eq_type= false; // =, <=> or IS NULL
  bool noeq_type= false; // < or >
  bool less_fl= false; // < or <=
  bool is_null= false;
  bool between= false;

  switch (((Item_func*) cond)->functype())
  {
  case Item_func::ISNULL_FUNC:
    is_null= 1;     /* fall through */
  case Item_func::EQ_FUNC:
  case Item_func::EQUAL_FUNC:
    eq_type= 1;
    break;
  case Item_func::LT_FUNC:
    noeq_type= 1;   /* fall through */
  case Item_func::LE_FUNC:
    less_fl= 1;
    break;
  case Item_func::GT_FUNC:
    noeq_type= 1;   /* fall through */
  case Item_func::GE_FUNC:
    break;
  case Item_func::BETWEEN:
    between= 1;
    break;
  case Item_func::MULT_EQUAL_FUNC:
    eq_type= 1;
    break;
  default:
    return 0; // Can't optimize function
  }

  Item *args[3];
  bool inv;

  /* Test if this is a comparison of a field and constant */
  if (! optimizer::simple_pred((Item_func*) cond, args, inv))
  {
    return 0;
  }

  if (inv && ! eq_type)
  {
    less_fl= 1 - less_fl; // Convert '<' -> '>' (etc)
  }

  /* Check if field is part of the tested partial key */
  unsigned char *key_ptr= ref->key_buff;
  KeyPartInfo *part= NULL;
  for (part= keyinfo->key_part; ; key_ptr+= part++->store_length)

  {
    if (part > field_part)
    {
      return 0;                     // Field is beyond the tested parts
    }
    if (part->field->eq(((Item_field*) args[0])->field))
    {
      break;                        // Found a part of the key for the field
    }
  }

  bool is_field_part= part == field_part;
  if (! (is_field_part || eq_type))
  {
    return 0;
  }

  key_part_map org_key_part_used= *key_part_used;
  if (eq_type || between || max_fl == less_fl)
  {
    uint32_t length= (key_ptr-ref->key_buff)+part->store_length;
    if (ref->key_length < length)
    {
    /* Ultimately ref->key_length will contain the length of the search key */
      ref->key_length= length;
      ref->key_parts= (part - keyinfo->key_part) + 1;
    }
    if (! *prefix_len && part + 1 == field_part)
    {
      *prefix_len= length;
    }
    if (is_field_part && eq_type)
    {
      *prefix_len= ref->key_length;
    }

    *key_part_used|= (key_part_map) 1 << (part - keyinfo->key_part);
  }

  if (org_key_part_used != *key_part_used ||
      (is_field_part &&
       (between || eq_type || max_fl == less_fl) && ! cond->val_int()))
  {
    /*
      It's the first predicate for this part or a predicate of the
      following form  that moves upper/lower bounds for max/min values:
      - field BETWEEN const AND const
      - field = const
      - field {<|<=} const, when searching for MAX
      - field {>|>=} const, when searching for MIN
    */

    if (is_null)
    {
      part->field->set_null();
      *key_ptr= (unsigned char) 1;
    }
    else
    {
      store_val_in_field(part->field, args[between && max_fl ? 2 : 1],
                         CHECK_FIELD_IGNORE);
      if (part->null_bit)
      {
        *key_ptr++= (unsigned char) test(part->field->is_null());
      }
      part->field->get_key_image(key_ptr, part->length);
    }
    if (is_field_part)
    {
      if (between || eq_type)
      {
        *range_fl&= ~(NO_MAX_RANGE | NO_MIN_RANGE);
      }
      else
      {
        *range_fl&= ~(max_fl ? NO_MAX_RANGE : NO_MIN_RANGE);
        if (noeq_type)
        {
          *range_fl|=  (max_fl ? NEAR_MAX : NEAR_MIN);
        }
        else
        {
          *range_fl&= ~(max_fl ? NEAR_MAX : NEAR_MIN);
        }
      }
    }
  }
  else if (eq_type)
  {
    if ((! is_null && !cond->val_int()) ||
        (is_null && !test(part->field->is_null())))
    {
     return 0;                       // Impossible test
    }
  }
  else if (is_field_part)
  {
    *range_fl&= ~(max_fl ? NO_MIN_RANGE : NO_MAX_RANGE);
  }
  return 1;
}


/**
  Check whether we can get value for {max|min}(field) by using a key.

     If where-condition is not a conjunction of 0 or more conjuct the
     function returns false, otherwise it checks whether there is an
     index including field as its k-th component/part such that:

     -# for each previous component f_i there is one and only one conjunct
        of the form: f_i= const_i or const_i= f_i or f_i is null
     -# references to field occur only in conjucts of the form:
        field {<|<=|>=|>|=} const or const {<|<=|>=|>|=} field or
        field BETWEEN const1 AND const2
     -# all references to the columns from the same table as column field
        occur only in conjucts mentioned above.
     -# each of k first components the index is not partial, i.e. is not
        defined on a fixed length proper prefix of the field.

     If such an index exists the function through the ref parameter
     returns the key value to find max/min for the field using the index,
     the length of first (k-1) components of the key and flags saying
     how to apply the key for the search max/min value.
     (if we have a condition field = const, prefix_len contains the length
     of the whole search key)

  @param[in]     max_fl      0 for MIN(field) / 1 for MAX(field)
  @param[in,out] ref         Reference to the structure we store the key value
  @param[in]     field       Field used inside MIN() / MAX()
  @param[in]     cond        WHERE condition
  @param[out]    range_fl    Bit flags for how to search if key is ok
  @param[out]    prefix_len  Length of prefix for the search range

  @note
    This function may set table->key_read to 1, which must be reset after
    index is used! (This can only happen when function returns 1)

  @retval
    0   Index can not be used to optimize MIN(field)/MAX(field)
  @retval
    1   Can use key to optimize MIN()/MAX().
    In this case ref, range_fl and prefix_len are updated
*/
static bool find_key_for_maxmin(bool max_fl,
                                table_reference_st *ref,
                                Field* field,
                                COND *cond,
                                uint32_t *range_fl,
                                uint32_t *prefix_len)
{
  if (! (field->flags & PART_KEY_FLAG))
  {
    return 0; // Not key field
  }

  Table *table= field->getTable();
  uint32_t idx= 0;

  KeyInfo *keyinfo,*keyinfo_end= NULL;
  for (keyinfo= table->key_info, keyinfo_end= keyinfo+table->getShare()->sizeKeys();
       keyinfo != keyinfo_end;
       keyinfo++,idx++)
  {
    KeyPartInfo *part= NULL;
    KeyPartInfo *part_end= NULL;
    key_part_map key_part_to_use= 0;
    /*
      Perform a check if index is not disabled by ALTER Table
      or IGNORE INDEX.
    */
    if (! table->keys_in_use_for_query.test(idx))
    {
      continue;
    }
    uint32_t jdx= 0;
    *prefix_len= 0;
    for (part= keyinfo->key_part, part_end= part+keyinfo->key_parts;
         part != part_end;
         part++, jdx++, key_part_to_use= (key_part_to_use << 1) | 1)
    {
      if (! (table->index_flags(idx) & HA_READ_ORDER))
      {
        return 0;
      }

      /* Check whether the index component is partial */
      Field *part_field= table->getField(part->fieldnr-1);
      part_field->setWriteSet();

      if ((part_field->flags & BLOB_FLAG) ||
          part->length < part_field->key_length())
      {
        break;
      }

      if (field->eq(part->field))
      {
        ref->key= idx;
        ref->key_length= 0;
        ref->key_parts= 0;
        key_part_map key_part_used= 0;
        *range_fl= NO_MIN_RANGE | NO_MAX_RANGE;
        if (matching_cond(max_fl,
                          ref,
                          keyinfo,
                          part,
                          cond,
                          &key_part_used,
                          range_fl,
                          prefix_len) &&
            ! (key_part_to_use & ~key_part_used))
        {
          if (! max_fl && key_part_used == key_part_to_use && part->null_bit)
          {
            /*
              The query is on this form:

              SELECT MIN(key_part_k)
              FROM t1
              WHERE key_part_1 = const and ... and key_part_k-1 = const

              If key_part_k is nullable, we want to find the first matching row
              where key_part_k is not null. The key buffer is now {const, ...,
              NULL}. This will be passed to the handler along with a flag
              indicating open interval. If a tuple is read that does not match
              these search criteria, an attempt will be made to read an exact
              match for the key buffer.
            */
            /* Set the first byte of key_part_k to 1, that means NULL */
            ref->key_buff[ref->key_length]= 1;
            ref->key_length+= part->store_length;
            ref->key_parts++;
            assert(ref->key_parts == jdx+1);
            *range_fl&= ~NO_MIN_RANGE;
            *range_fl|= NEAR_MIN; // Open interval
          }
          /*
            The following test is false when the key in the key tree is
            converted (for example to upper case)
          */
          if (field->part_of_key.test(idx))
          {
            table->key_read= 1;
            table->cursor->extra(HA_EXTRA_KEYREAD);
          }
          return 1;
        }
      }
    }
  }
  return 0;
}


/**
  Check whether found key is in range specified by conditions.

  @param[in] max_fl         0 for MIN(field) / 1 for MAX(field)
  @param[in] ref            Reference to the key value and info
  @param[in] field          Field used the MIN/MAX expression
  @param[in] cond           WHERE condition
  @param[in] range_fl       Says whether there is a condition to to be checked
  @param[in] prefix_len     Length of the constant part of the key

  @retval
    0        ok
  @retval
    1        WHERE was not true for the found row
*/
static int reckey_in_range(bool max_fl,
                           table_reference_st *ref,
                           Field* field,
                           COND *cond,
                           uint32_t range_fl,
                           uint32_t prefix_len)
{
  if (key_cmp_if_same(field->getTable(), ref->key_buff, ref->key, prefix_len))
  {
    return 1;
  }
  if (! cond || (range_fl & (max_fl ? NO_MIN_RANGE : NO_MAX_RANGE)))
  {
    return 0;
  }
  return maxmin_in_range(max_fl, field, cond);
}


/**
  Check whether {MAX|MIN}(field) is in range specified by conditions.

  @param[in] max_fl          0 for MIN(field) / 1 for MAX(field)
  @param[in] field           Field used the MIN/MAX expression
  @param[in] cond            WHERE condition

  @retval
    0        ok
  @retval
    1        WHERE was not true for the found row
*/
static int maxmin_in_range(bool max_fl, Field* field, COND *cond)
{
  /* If AND/OR condition */
  if (cond->type() == Item::COND_ITEM)
  {
    List<Item>::iterator li(((Item_cond*) cond)->argument_list()->begin());
    Item *item;
    while ((item= li++))
    {
      if (maxmin_in_range(max_fl, field, item))
      {
        return 1;
      }
    }
    return 0;
  }

  if (cond->used_tables() != field->getTable()->map)
  {
    return 0;
  }
  bool less_fl= false;
  switch (((Item_func*) cond)->functype())
  {
  case Item_func::BETWEEN:
    return cond->val_int() == 0;                // Return 1 if WHERE is false
  case Item_func::LT_FUNC:
  case Item_func::LE_FUNC:
    less_fl= 1;
  case Item_func::GT_FUNC:
  case Item_func::GE_FUNC:
  {
    Item *item= ((Item_func*) cond)->arguments()[1];
    /* In case of 'const op item' we have to swap the operator */
    if (! item->const_item())
    {
      less_fl= 1-less_fl;
    }
    /*
      We only have to check the expression if we are using an expression like
      SELECT MAX(b) FROM t1 WHERE a=const AND b>const
      not for
      SELECT MAX(b) FROM t1 WHERE a=const AND b<const
    */
    if (max_fl != less_fl)
    {
      return cond->val_int() == 0;                // Return 1 if WHERE is false
    }
    return 0;
  }
  case Item_func::EQ_FUNC:
  case Item_func::EQUAL_FUNC:
    break;
  default:                                        // Keep compiler happy
    assert(1);                               // Impossible
    break;
  }
  return 0;
}

} /* namespace drizzled */

