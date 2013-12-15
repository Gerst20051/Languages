/* Copyright (C) 2000 MySQL AB

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA */

/**
  @file

  @brief
  subselect Item

  @todo
    - add function from select_query that use JOIN* as parameter to JOIN
    methods (sql_select.h/sql_select.cc)
*/
#include <config.h>

#include <cstdio>
#include <limits.h>

#include <drizzled/session.h>
#include <drizzled/sql_select.h>
#include <drizzled/error.h>
#include <drizzled/item/cache.h>
#include <drizzled/item/subselect.h>
#include <drizzled/item/cmpfunc.h>
#include <drizzled/item/ref_null_helper.h>
#include <drizzled/cached_item.h>
#include <drizzled/check_stack_overrun.h>
#include <drizzled/item/ref_null_helper.h>
#include <drizzled/item/direct_ref.h>
#include <drizzled/join.h>
#include <drizzled/plugin/storage_engine.h>
#include <drizzled/select_singlerow_subselect.h>
#include <drizzled/select_max_min_finder_subselect.h>
#include <drizzled/select_exists_subselect.h>
#include <drizzled/select_union.h>
#include <drizzled/sql_lex.h>

namespace drizzled {

extern plugin::StorageEngine *myisam_engine;

inline Item * and_items(Item* cond, Item *item)
{
  return (cond? (new Item_cond_and(cond, item)) : item);
}

Item_subselect::Item_subselect() :
  Item_result_field(),
  value_assigned(false),
  session(NULL),
  substitution(NULL),
  unit(NULL),
  engine(NULL),
  old_engine(NULL),
  used_tables_cache(0),
  max_columns(0),
  parsing_place(NO_MATTER),
  have_to_be_excluded(false),
  const_item_cache(true),
  engine_changed(false),
  changed(false),
  is_correlated(false)
{
  with_subselect= 1;
  reset();
  /*
    Item value is NULL if select_result_interceptor didn't change this value
    (i.e. some rows will be found returned)
  */
  null_value= 1;
}


void Item_subselect::init(Select_Lex *select_lex,
			  select_result_interceptor *result)
{
  /*
    Please see Item_singlerow_subselect::invalidate_and_restore_select_lex(),
    which depends on alterations to the parse tree implemented here.
  */

  unit= select_lex->master_unit();

  if (unit->item)
  {
    /*
      Item can be changed in JOIN::prepare while engine in Join::optimize
      => we do not copy old_engine here
    */
    engine= unit->item->engine;
    parsing_place= unit->item->parsing_place;
    unit->item->engine= 0;
    unit->item= this;
    engine->change_result(this, result);
  }
  else
  {
    Select_Lex *outer_select= unit->outer_select();
    /*
      do not take into account expression inside aggregate functions because
      they can access original table fields
    */
    parsing_place= (outer_select->in_sum_expr ?
                    NO_MATTER :
                    outer_select->parsing_place);
    if (unit->is_union())
      engine= new subselect_union_engine(unit, result, this);
    else
      engine= new subselect_single_select_engine(select_lex, result, this);
  }
  {
    Select_Lex *upper= unit->outer_select();
    if (upper->parsing_place == IN_HAVING)
      upper->subquery_in_having= 1;
  }
  return;
}

Select_Lex *
Item_subselect::get_select_lex()
{
  return unit->first_select();
}

void Item_subselect::cleanup()
{
  Item_result_field::cleanup();
  if (old_engine)
  {
    if (engine)
      engine->cleanup();
    engine= old_engine;
    old_engine= 0;
  }
  if (engine)
    engine->cleanup();
  reset();
  value_assigned= 0;
  return;
}

void Item_singlerow_subselect::cleanup()
{
  value= 0; row= 0;
  Item_subselect::cleanup();
  return;
}


void Item_in_subselect::cleanup()
{
  if (left_expr_cache)
  {
    left_expr_cache->delete_elements();
    delete left_expr_cache;
    left_expr_cache= NULL;
  }
  first_execution= true;
  Item_subselect::cleanup();
  return;
}

Item_subselect::~Item_subselect()
{
  delete engine;
}

Item_subselect::trans_res
Item_subselect::select_transformer(Join *)
{
  return(RES_OK);
}


bool Item_subselect::fix_fields(Session *session_param, Item **ref)
{
  char const *save_where= session_param->where();
  bool res;

  assert(fixed == 0);
  engine->set_session((session= session_param));

  if (check_stack_overrun(session, STACK_MIN_SIZE, (unsigned char*)&res))
    return true;

  res= engine->prepare();

  // all transformation is done (used by prepared statements)
  changed= 1;

  if (!res)
  {
    /*
      Substitute the current item with an Item_in_optimizer that was
      created by Item_in_subselect::select_in_like_transformer and
      call fix_fields for the substituted item which in turn calls
      engine->prepare for the subquery predicate.
    */
    if (substitution)
    {
      int ret= 0;

      // did we changed top item of WHERE condition
      if (unit->outer_select()->where == (*ref))
      {
        unit->outer_select()->where= substitution; // correct WHERE for PS
      }
      else if (unit->outer_select()->having == (*ref))
      {
        unit->outer_select()->having= substitution; // correct HAVING for PS
      }

      (*ref)= substitution;
      substitution->name= name;
      if (have_to_be_excluded)
      {
        engine->exclude();
      }
      substitution= 0;
      session->setWhere("checking transformed subquery");
      if (! (*ref)->fixed)
      {
        ret= (*ref)->fix_fields(session, ref);
      }
      session->setWhere(save_where);

      return ret;
    }
    // Is it one field subselect?
    if (engine->cols() > max_columns)
    {
      my_error(ER_OPERAND_COLUMNS, MYF(0), 1);
      return true;
    }
    fix_length_and_dec();
  }
  else
    goto err;

  if (engine->uncacheable())
  {
    const_item_cache= false;
    if (engine->uncacheable(UNCACHEABLE_RAND))
    {
      used_tables_cache|= RAND_TABLE_BIT;
    }
  }
  fixed= 1;

err:
  session->setWhere(save_where);
  return res;
}


bool Item_subselect::walk(Item_processor processor, bool walk_subquery,
                          unsigned char *argument)
{

  if (walk_subquery)
  {
    for (Select_Lex *lex= unit->first_select(); lex; lex= lex->next_select())
    {
      List<Item>::iterator li(lex->item_list.begin());
      Item *item;
      Order *order;

      if (lex->where && (lex->where)->walk(processor, walk_subquery, argument))
        return 1;
      if (lex->having && (lex->having)->walk(processor, walk_subquery,
                                             argument))
        return 1;

      while ((item=li++))
      {
        if (item->walk(processor, walk_subquery, argument))
          return 1;
      }
      for (order= (Order*) lex->order_list.first ; order; order= order->next)
      {
        if ((*order->item)->walk(processor, walk_subquery, argument))
          return 1;
      }
      for (order= (Order*) lex->group_list.first ; order; order= order->next)
      {
        if ((*order->item)->walk(processor, walk_subquery, argument))
          return 1;
      }
    }
  }
  return (this->*processor)(argument);
}


bool Item_subselect::exec()
{
  int res;

  if (session->is_error())
  /* Do not execute subselect in case of a fatal error */
    return 1;

  res= engine->exec();

  if (engine_changed)
  {
    engine_changed= 0;
    return exec();
  }
  return (res);
}


/*
  Compute the IN predicate if the left operand's cache changed.
*/

bool Item_in_subselect::exec()
{
  assert(exec_method != MATERIALIZATION ||
              (exec_method == MATERIALIZATION &&
               engine->engine_type() == subselect_engine::HASH_SJ_ENGINE));
  /*
    Initialize the cache of the left predicate operand. This has to be done as
    late as now, because Cached_item directly contains a resolved field (not
    an item, and in some cases (when temp tables are created), these fields
    end up pointing to the wrong field. One solution is to change Cached_item
    to not resolve its field upon creation, but to resolve it dynamically
    from a given Item_ref object.
    TODO: the cache should be applied conditionally based on:
    - rules - e.g. only if the left operand is known to be ordered, and/or
    - on a cost-based basis, that takes into account the cost of a cache
      lookup, the cache hit rate, and the savings per cache hit.
  */
  if (!left_expr_cache && exec_method == MATERIALIZATION)
    init_left_expr_cache();

  /* If the new left operand is already in the cache, reuse the old result. */
  if (left_expr_cache && test_if_item_cache_changed(*left_expr_cache) < 0)
  {
    /* Always compute IN for the first row as the cache is not valid for it. */
    if (!first_execution)
      return(false);
    first_execution= false;
  }

  /*
    The exec() method below updates item::value, and item::null_value, thus if
    we don't call it, the next call to item::val_int() will return whatever
    result was computed by its previous call.
  */
  return(Item_subselect::exec());
}


Item::Type Item_subselect::type() const
{
  return SUBSELECT_ITEM;
}


void Item_subselect::fix_length_and_dec()
{
  engine->fix_length_and_dec(0);
}


table_map Item_subselect::used_tables() const
{
  return (table_map) (engine->uncacheable() ? used_tables_cache : 0L);
}


bool Item_subselect::const_item() const
{
  return const_item_cache;
}

Item *Item_subselect::get_tmp_table_item(Session *session_arg)
{
  if (!with_sum_func && !const_item())
    return new Item_field(result_field);
  return copy_or_same(session_arg);
}

void Item_subselect::update_used_tables()
{
  if (! engine->uncacheable())
  {
    // did all used tables become static?
    if (!(used_tables_cache & ~engine->upper_select_const_tables()))
      const_item_cache= true;
  }
}


void Item_subselect::print(String *str)
{
  str->append('(');
  engine->print(str);
  str->append(')');
}


Item_singlerow_subselect::Item_singlerow_subselect(Select_Lex *select_lex)
  :Item_subselect(), value(0)
{
  init(select_lex, new select_singlerow_subselect(this));
  maybe_null= 1;
  max_columns= UINT_MAX;
  return;
}

Select_Lex *
Item_singlerow_subselect::invalidate_and_restore_select_lex()
{
  Select_Lex *result= get_select_lex();

  assert(result);

  /*
    This code restore the parse tree in it's state before the execution of
    Item_singlerow_subselect::Item_singlerow_subselect(),
    and in particular decouples this object from the Select_Lex,
    so that the Select_Lex can be used with a different flavor
    or Item_subselect instead, as part of query rewriting.
  */
  unit->item= NULL;

  return(result);
}

Item_maxmin_subselect::Item_maxmin_subselect(Session *session_param,
                                             Item_subselect *parent,
					     Select_Lex *select_lex,
					     bool max_arg)
  :Item_singlerow_subselect(), was_values(true)
{
  max= max_arg;
  init(select_lex, new select_max_min_finder_subselect(this, max_arg));
  max_columns= 1;
  maybe_null= 1;
  max_columns= 1;

  /*
    Following information was collected during performing fix_fields()
    of Items belonged to subquery, which will be not repeated
  */
  used_tables_cache= parent->get_used_tables_cache();
  const_item_cache= parent->get_const_item_cache();

  /*
    this subquery always creates during preparation, so we can assign
    session here
  */
  session= session_param;

  return;
}

void Item_maxmin_subselect::cleanup()
{
  Item_singlerow_subselect::cleanup();

  /*
    By default it is true to avoid true reporting by
    Item_func_not_all/Item_func_nop_all if this item was never called.

    Engine exec() set it to false by reset_value_registration() call.
    select_max_min_finder_subselect::send_data() set it back to true if some
    value will be found.
  */
  was_values= true;
  return;
}


void Item_maxmin_subselect::print(String *str)
{
  str->append(max?"<max>":"<min>", 5);
  Item_singlerow_subselect::print(str);
}


void Item_singlerow_subselect::reset()
{
  null_value= 1;
  if (value)
    value->null_value= 1;
}


/**
  @todo
  - We cant change name of Item_field or Item_ref, because it will
  prevent it's correct resolving, but we should save name of
  removed item => we do not make optimization if top item of
  list is field or reference.
  - switch off this optimization for prepare statement,
  because we do not rollback this changes.
  Make rollback for it, or special name resolving mode in 5.0.
*/
Item_subselect::trans_res
Item_singlerow_subselect::select_transformer(Join *join)
{
  if (changed)
    return(RES_OK);

  Select_Lex *select_lex= join->select_lex;

  if (!select_lex->master_unit()->is_union() &&
      !select_lex->table_list.elements &&
      select_lex->item_list.size() == 1 &&
      !select_lex->item_list.front().with_sum_func &&
      /*
	We cant change name of Item_field or Item_ref, because it will
	prevent it's correct resolving, but we should save name of
	removed item => we do not make optimization if top item of
	list is field or reference.
	TODO: solve above problem
      */
      !(select_lex->item_list.front().type() == FIELD_ITEM ||
	select_lex->item_list.front().type() == REF_ITEM) &&
      !join->conds && !join->having
      )
  {

    have_to_be_excluded= 1;
    if (session->lex().describe)
    {
      char warn_buff[DRIZZLE_ERRMSG_SIZE];
      snprintf(warn_buff, sizeof(warn_buff), ER(ER_SELECT_REDUCED), select_lex->select_number);
      push_warning(session, DRIZZLE_ERROR::WARN_LEVEL_NOTE,
		   ER_SELECT_REDUCED, warn_buff);
    }
    substitution= &select_lex->item_list.front();
    /*
      as far as we moved content to upper level, field which depend of
      'upper' select is not really dependent => we remove this dependence
    */
    substitution->walk(&Item::remove_dependence_processor, 0,
		       (unsigned char *) select_lex->outer_select());
    return(RES_REDUCE);
  }
  return(RES_OK);
}


void Item_singlerow_subselect::store(uint32_t i, Item *item)
{
  row[i]->store(item);
}

enum Item_result Item_singlerow_subselect::result_type() const
{
  return engine->type();
}

/*
 Don't rely on the result type to calculate field type.
 Ask the engine instead.
*/
enum_field_types Item_singlerow_subselect::field_type() const
{
  return engine->field_type();
}

void Item_singlerow_subselect::fix_length_and_dec()
{
  if ((max_columns= engine->cols()) == 1)
  {
    engine->fix_length_and_dec(row= &value);
  }
  else
  {
    if (!(row= (Item_cache**) memory::sql_alloc(sizeof(Item_cache*)*max_columns)))
      return;
    engine->fix_length_and_dec(row);
    value= *row;
  }
  unsigned_flag= value->unsigned_flag;
  /*
    If there are not tables in subquery then ability to have NULL value
    depends on SELECT list (if single row subquery have tables then it
    always can be NULL if there are not records fetched).
  */
  if (engine->no_tables())
    maybe_null= engine->may_be_null();
}

uint32_t Item_singlerow_subselect::cols()
{
  return engine->cols();
}

bool Item_singlerow_subselect::check_cols(uint32_t c)
{
  if (c != engine->cols())
  {
    my_error(ER_OPERAND_COLUMNS, MYF(0), c);
    return 1;
  }
  return 0;
}

bool Item_singlerow_subselect::null_inside()
{
  for (uint32_t i= 0; i < max_columns ; i++)
  {
    if (row[i]->null_value)
      return 1;
  }
  return 0;
}

void Item_singlerow_subselect::bring_value()
{
  exec();
}

double Item_singlerow_subselect::val_real()
{
  assert(fixed == 1);
  if (!exec() && !value->null_value)
  {
    null_value= 0;
    return value->val_real();
  }
  else
  {
    reset();
    return 0;
  }
}

int64_t Item_singlerow_subselect::val_int()
{
  assert(fixed == 1);
  if (!exec() && !value->null_value)
  {
    null_value= 0;
    return value->val_int();
  }
  else
  {
    reset();
    return 0;
  }
}

String *Item_singlerow_subselect::val_str(String *str)
{
  if (!exec() && !value->null_value)
  {
    null_value= 0;
    return value->val_str(str);
  }
  else
  {
    reset();
    return 0;
  }
}


type::Decimal *Item_singlerow_subselect::val_decimal(type::Decimal *decimal_value)
{
  if (!exec() && !value->null_value)
  {
    null_value= 0;
    return value->val_decimal(decimal_value);
  }
  else
  {
    reset();
    return 0;
  }
}


bool Item_singlerow_subselect::val_bool()
{
  if (!exec() && !value->null_value)
  {
    null_value= 0;
    return value->val_bool();
  }
  else
  {
    reset();
    return 0;
  }
}


Item_exists_subselect::Item_exists_subselect(Select_Lex *select_lex):
  Item_subselect()
{
  bool val_bool();
  init(select_lex, new select_exists_subselect(this));
  max_columns= UINT_MAX;
  null_value= 0; //can't be NULL
  maybe_null= 0; //can't be NULL
  value= 0;
  return;
}


void Item_exists_subselect::print(String *str)
{
  str->append(STRING_WITH_LEN("exists"));
  Item_subselect::print(str);
}


bool Item_in_subselect::test_limit(Select_Lex_Unit *unit_arg)
{
  if (unit_arg->fake_select_lex &&
      unit_arg->fake_select_lex->test_limit())
    return(1);

  Select_Lex *sl= unit_arg->first_select();
  for (; sl; sl= sl->next_select())
  {
    if (sl->test_limit())
      return(1);
  }
  return(0);
}

Item_in_subselect::Item_in_subselect(Item * left_exp,
                                     Select_Lex *select_lex) :
  Item_exists_subselect(),
  left_expr(left_exp),
  left_expr_cache(NULL),
  first_execution(true),
  optimizer(NULL),
  pushed_cond_guards(NULL),
  sj_convert_priority(0),
  expr_join_nest(NULL),
  exec_method(NOT_TRANSFORMED),
  upper_item(NULL)
{
  init(select_lex, new select_exists_subselect(this));
  max_columns= UINT_MAX;
  maybe_null= 1;
  abort_on_null= 0;
  reset();
  //if test_limit will fail then error will be reported to client
  test_limit(select_lex->master_unit());
  return;
}

Item_allany_subselect::Item_allany_subselect(Item * left_exp,
                                             chooser_compare_func_creator fc,
					     Select_Lex *select_lex,
					     bool all_arg)
  :Item_in_subselect(), func_creator(fc), all(all_arg)
{
  left_expr= left_exp;
  func= func_creator(all_arg);
  init(select_lex, new select_exists_subselect(this));
  max_columns= 1;
  abort_on_null= 0;
  reset();
  //if test_limit will fail then error will be reported to client
  test_limit(select_lex->master_unit());
  return;
}


void Item_exists_subselect::fix_length_and_dec()
{
   decimals= 0;
   max_length= 1;
   max_columns= engine->cols();
  /* We need only 1 row to determine existence */
  unit->global_parameters->select_limit= new Item_int((int32_t) 1);
}

double Item_exists_subselect::val_real()
{
  assert(fixed == 1);
  if (exec())
  {
    reset();
    return 0;
  }
  return (double) value;
}

int64_t Item_exists_subselect::val_int()
{
  assert(fixed == 1);
  if (exec())
  {
    reset();
    return 0;
  }
  return value;
}

String *Item_exists_subselect::val_str(String *str)
{
  assert(fixed == 1);
  if (exec())
  {
    reset();
    return 0;
  }
  str->set((uint64_t)value,&my_charset_bin);
  return str;
}


type::Decimal *Item_exists_subselect::val_decimal(type::Decimal *decimal_value)
{
  assert(fixed == 1);
  if (exec())
  {
    reset();
    return 0;
  }
  int2_class_decimal(E_DEC_FATAL_ERROR, value, 0, decimal_value);
  return decimal_value;
}


bool Item_exists_subselect::val_bool()
{
  assert(fixed == 1);
  if (exec())
  {
    reset();
    return 0;
  }
  return value != 0;
}


double Item_in_subselect::val_real()
{
  /*
    As far as Item_in_subselect called only from Item_in_optimizer this
    method should not be used
  */
  assert(0);
  assert(fixed == 1);
  null_value= 0;
  if (exec())
  {
    reset();
    null_value= 1;
    return 0;
  }
  if (was_null && !value)
    null_value= 1;
  return (double) value;
}


int64_t Item_in_subselect::val_int()
{
  /*
    As far as Item_in_subselect called only from Item_in_optimizer this
    method should not be used
  */
  assert(fixed == 1);
  null_value= 0;
  if (exec())
  {
    reset();
    null_value= 1;
    return 0;
  }
  if (was_null && !value)
    null_value= 1;
  return value;
}


String *Item_in_subselect::val_str(String *str)
{
  /*
    As far as Item_in_subselect called only from Item_in_optimizer this
    method should not be used
  */
  assert(0);
  assert(fixed == 1);
  null_value= 0;
  if (exec())
  {
    reset();
    null_value= 1;
    return 0;
  }
  if (was_null && !value)
  {
    null_value= 1;
    return 0;
  }
  str->set((uint64_t)value, &my_charset_bin);
  return str;
}


bool Item_in_subselect::val_bool()
{
  assert(fixed == 1);
  null_value= 0;
  if (exec())
  {
    reset();
    /*
      Must mark the IN predicate as NULL so as to make sure an enclosing NOT
      predicate will return false. See the comments in
      subselect_uniquesubquery_engine::copy_ref_key for further details.
    */
    null_value= 1;
    return 0;
  }
  if (was_null && !value)
    null_value= 1;
  return value;
}

type::Decimal *Item_in_subselect::val_decimal(type::Decimal *decimal_value)
{
  /*
    As far as Item_in_subselect called only from Item_in_optimizer this
    method should not be used
  */
  assert(0);
  null_value= 0;
  assert(fixed == 1);
  if (exec())
  {
    reset();
    null_value= 1;
    return 0;
  }
  if (was_null && !value)
    null_value= 1;
  int2_class_decimal(E_DEC_FATAL_ERROR, value, 0, decimal_value);
  return decimal_value;
}


/*
  Rewrite a single-column IN/ALL/ANY subselect

  SYNOPSIS
    Item_in_subselect::single_value_transformer()
      join  Join object of the subquery (i.e. 'child' join).
      func  Subquery comparison creator

  DESCRIPTION
    Rewrite a single-column subquery using rule-based approach. The subquery

       oe $cmp$ (SELECT ie FROM ... WHERE subq_where ... HAVING subq_having)

    First, try to convert the subquery to scalar-result subquery in one of
    the forms:

       - oe $cmp$ (SELECT MAX(...) )  // handled by Item_singlerow_subselect
       - oe $cmp$ <max>(SELECT ...)   // handled by Item_maxmin_subselect

    If that fails, the subquery will be handled with class Item_in_optimizer.
    There are two possibilites:
    - If the subquery execution method is materialization, then the subquery is
      not transformed any further.
    - Otherwise the IN predicates is transformed into EXISTS by injecting
      equi-join predicates and possibly other helper predicates. For details
      see method single_value_in_like_transformer().

  RETURN
    RES_OK     Either subquery was transformed, or appopriate
                       predicates where injected into it.
    RES_REDUCE The subquery was reduced to non-subquery
    RES_ERROR  Error
*/

Item_subselect::trans_res
Item_in_subselect::single_value_transformer(Join *join,
					    const Comp_creator *func)
{
  Select_Lex *select_lex= join->select_lex;

  /*
    Check that the right part of the subselect contains no more than one
    column. E.g. in SELECT 1 IN (SELECT * ..) the right part is (SELECT * ...)
  */
  if (select_lex->item_list.size() > 1)
  {
    my_error(ER_OPERAND_COLUMNS, MYF(0), 1);
    return(RES_ERROR);
  }

  /*
    If this is an ALL/ANY single-value subselect, try to rewrite it with
    a MIN/MAX subselect. We can do that if a possible NULL result of the
    subselect can be ignored.
    E.g. SELECT * FROM t1 WHERE b > ANY (SELECT a FROM t2) can be rewritten
    with SELECT * FROM t1 WHERE b > (SELECT MAX(a) FROM t2).
    We can't check that this optimization is safe if it's not a top-level
    item of the WHERE clause (e.g. because the WHERE clause can contain IS
    NULL/IS NOT NULL functions). If so, we rewrite ALL/ANY with NOT EXISTS
    later in this method.
  */
  if ((abort_on_null || (upper_item && upper_item->top_level())) &&
      select_lex->master_unit()->uncacheable.none() && !func->eqne_op())
  {
    if (substitution)
    {
      // It is second (third, ...) SELECT of UNION => All is done
      return(RES_OK);
    }

    Item *subs;
    if (!select_lex->group_list.elements &&
        !select_lex->having &&
	!select_lex->with_sum_func &&
	!(select_lex->next_select()) &&
        select_lex->table_list.elements)
    {
      Item_sum_hybrid *item;
      nesting_map save_allow_sum_func;
      if (func->l_op())
      {
	/*
	  (ALL && (> || =>)) || (ANY && (< || =<))
	  for ALL condition is inverted
	*/
	item= new Item_sum_max(*select_lex->ref_pointer_array);
      }
      else
      {
	/*
	  (ALL && (< || =<)) || (ANY && (> || =>))
	  for ALL condition is inverted
	*/
	item= new Item_sum_min(*select_lex->ref_pointer_array);
      }
      if (upper_item)
        upper_item->set_sum_test(item);
      *select_lex->ref_pointer_array= item;
      {
	List<Item>::iterator it(select_lex->item_list.begin());
	it++;
	it.replace(item);
      }

      save_allow_sum_func= session->lex().allow_sum_func;
      session->lex().allow_sum_func|= 1 << session->lex().current_select->nest_level;
      /*
	Item_sum_(max|min) can't substitute other item => we can use 0 as
        reference, also Item_sum_(max|min) can't be fixed after creation, so
        we do not check item->fixed
      */
      if (item->fix_fields(session, 0))
	return(RES_ERROR);
      session->lex().allow_sum_func= save_allow_sum_func;
      /* we added aggregate function => we have to change statistic */
      count_field_types(select_lex, &join->tmp_table_param, join->all_fields,
                        0);

      subs= new Item_singlerow_subselect(select_lex);
    }
    else
    {
      Item_maxmin_subselect *item;
      subs= item= new Item_maxmin_subselect(session, this, select_lex, func->l_op());
      if (upper_item)
        upper_item->set_sub_test(item);
    }
    /* fix fields is already called for  left expression */
    substitution= func->create(left_expr, subs);
    return(RES_OK);
  }

  if (!substitution)
  {
    /* We're invoked for the 1st (or the only) SELECT in the subquery UNION */
    Select_Lex_Unit *master_unit= select_lex->master_unit();
    substitution= optimizer;

    Select_Lex *current= session->lex().current_select, *up;

    session->lex().current_select= up= current->return_after_parsing();
    //optimizer never use Item **ref => we can pass 0 as parameter
    if (!optimizer || optimizer->fix_left(session, 0))
    {
      session->lex().current_select= current;
      return(RES_ERROR);
    }
    session->lex().current_select= current;

    /*
      As far as  Item_ref_in_optimizer do not substitute itself on fix_fields
      we can use same item for all selects.
    */
    expr= new Item_direct_ref(&select_lex->context,
                              (Item**)optimizer->get_cache(),
			      (char *)"<no matter>",
			      (char *)in_left_expr_name);

    master_unit->uncacheable.set(UNCACHEABLE_DEPENDENT);
  }

  if (!abort_on_null && left_expr->maybe_null && !pushed_cond_guards)
  {
    if (!(pushed_cond_guards= (bool*)join->session->getMemRoot()->allocate(sizeof(bool))))
      return(RES_ERROR);
    pushed_cond_guards[0]= true;
  }

  /*
    If this IN predicate can be computed via materialization, do not
    perform the IN -> EXISTS transformation.
  */
  if (exec_method == MATERIALIZATION)
    return(RES_OK);

  /* Perform the IN=>EXISTS transformation. */
  return(single_value_in_to_exists_transformer(join, func));
}


/**
  Transofrm an IN predicate into EXISTS via predicate injection.

  @details The transformation injects additional predicates into the subquery
  (and makes the subquery correlated) as follows.

  - If the subquery has aggregates, GROUP BY, or HAVING, convert to

    SELECT ie FROM ...  HAVING subq_having AND
                               trigcond(oe $cmp$ ref_or_null_helper<ie>)

    the addition is wrapped into trigger only when we want to distinguish
    between NULL and false results.

  - Otherwise (no aggregates/GROUP BY/HAVING) convert it to one of the
    following:

    = If we don't need to distinguish between NULL and false subquery:

      SELECT 1 FROM ... WHERE (oe $cmp$ ie) AND subq_where

    = If we need to distinguish between those:

      SELECT 1 FROM ...
        WHERE  subq_where AND trigcond((oe $cmp$ ie) OR (ie IS NULL))
        HAVING trigcond(<is_not_null_test>(ie))

    @param join  Join object of the subquery (i.e. 'child' join).
    @param func  Subquery comparison creator

    @retval RES_OK     Either subquery was transformed, or appopriate
                       predicates where injected into it.
    @retval RES_REDUCE The subquery was reduced to non-subquery
    @retval RES_ERROR  Error
*/

Item_subselect::trans_res
Item_in_subselect::single_value_in_to_exists_transformer(Join * join, const Comp_creator *func)
{
  Select_Lex *select_lex= join->select_lex;

  select_lex->uncacheable.set(UNCACHEABLE_DEPENDENT);
  if (join->having || select_lex->with_sum_func ||
      select_lex->group_list.elements)
  {
    bool tmp;
    Item *item= func->create(expr,
                             new Item_ref_null_helper(&select_lex->context,
                                                      this,
                                                      select_lex->
                                                      ref_pointer_array,
                                                      (char *)"<ref>",
                                                      this->full_name()));
    if (!abort_on_null && left_expr->maybe_null)
    {
      /*
        We can encounter "NULL IN (SELECT ...)". Wrap the added condition
        within a trig_cond.
      */
      item= new Item_func_trig_cond(item, get_cond_guard(0));
    }

    /*
      AND and comparison functions can't be changed during fix_fields()
      we can assign select_lex->having here, and pass 0 as last
      argument (reference) to fix_fields()
    */
    select_lex->having= join->having= and_items(join->having, item);
    if (join->having == item)
      item->name= (char*)in_having_cond;
    select_lex->having->top_level_item();
    select_lex->having_fix_field= 1;
    /*
      we do not check join->having->fixed, because Item_and (from and_items)
      or comparison function (from func->create) can't be fixed after creation
    */
    tmp= join->having->fix_fields(session, 0);
    select_lex->having_fix_field= 0;
    if (tmp)
      return(RES_ERROR);
  }
  else
  {
    Item *item= &select_lex->item_list.front();

    if (select_lex->table_list.elements)
    {
      bool tmp;
      Item *having= item, *orig_item= item;
      select_lex->item_list.clear();
      select_lex->item_list.push_back(new Item_int("Not_used",
                                                   (int64_t) 1,
                                                   MY_INT64_NUM_DECIMAL_DIGITS));
      select_lex->ref_pointer_array[0]= &select_lex->item_list.front();

      item= func->create(expr, item);
      if (!abort_on_null && orig_item->maybe_null)
      {
	having= new Item_is_not_null_test(this, having);
        if (left_expr->maybe_null)
        {
          if (!(having= new Item_func_trig_cond(having,
                                                get_cond_guard(0))))
            return(RES_ERROR);
        }
	/*
	  Item_is_not_null_test can't be changed during fix_fields()
	  we can assign select_lex->having here, and pass 0 as last
	  argument (reference) to fix_fields()
	*/
        having->name= (char*)in_having_cond;
	select_lex->having= join->having= having;
	select_lex->having_fix_field= 1;
        /*
          we do not check join->having->fixed, because Item_and (from
          and_items) or comparison function (from func->create) can't be
          fixed after creation
        */
	tmp= join->having->fix_fields(session, 0);
        select_lex->having_fix_field= 0;
        if (tmp)
	  return(RES_ERROR);
	item= new Item_cond_or(item,
			       new Item_func_isnull(orig_item));
      }
      /*
        If we may encounter NULL IN (SELECT ...) and care whether subquery
        result is NULL or false, wrap condition in a trig_cond.
      */
      if (!abort_on_null && left_expr->maybe_null)
      {
        if (!(item= new Item_func_trig_cond(item, get_cond_guard(0))))
          return(RES_ERROR);
      }
      /*
        TODO: figure out why the following is done here in
        single_value_transformer but there is no corresponding action in
        row_value_transformer?
      */
      item->name= (char *)in_additional_cond;

      /*
	AND can't be changed during fix_fields()
	we can assign select_lex->having here, and pass 0 as last
	argument (reference) to fix_fields()
      */
      select_lex->where= join->conds= and_items(join->conds, item);
      select_lex->where->top_level_item();
      /*
        we do not check join->conds->fixed, because Item_and can't be fixed
        after creation
      */
      if (join->conds->fix_fields(session, 0))
	return(RES_ERROR);
    }
    else
    {
      bool tmp;
      if (select_lex->master_unit()->is_union())
      {
	/*
	  comparison functions can't be changed during fix_fields()
	  we can assign select_lex->having here, and pass 0 as last
	  argument (reference) to fix_fields()
	*/
        Item *new_having=
          func->create(expr,
                       new Item_ref_null_helper(&select_lex->context, this,
                                            select_lex->ref_pointer_array,
                                            (char *)"<no matter>",
                                            (char *)"<result>"));
        if (!abort_on_null && left_expr->maybe_null)
        {
          if (!(new_having= new Item_func_trig_cond(new_having,
                                                    get_cond_guard(0))))
            return(RES_ERROR);
        }
        new_having->name= (char*)in_having_cond;
	select_lex->having= join->having= new_having;
	select_lex->having_fix_field= 1;

        /*
          we do not check join->having->fixed, because comparison function
          (from func->create) can't be fixed after creation
        */
	tmp= join->having->fix_fields(session, 0);
        select_lex->having_fix_field= 0;
        if (tmp)
	  return(RES_ERROR);
      }
      else
      {
	// it is single select without tables => possible optimization
	item= func->create(left_expr, item);
	// fix_field of item will be done in time of substituting
	substitution= item;
	have_to_be_excluded= 1;
	if (session->lex().describe)
	{
	  char warn_buff[DRIZZLE_ERRMSG_SIZE];
	  snprintf(warn_buff, sizeof(warn_buff), ER(ER_SELECT_REDUCED), select_lex->select_number);
	  push_warning(session, DRIZZLE_ERROR::WARN_LEVEL_NOTE,
		       ER_SELECT_REDUCED, warn_buff);
	}
	return(RES_REDUCE);
      }
    }
  }

  return(RES_OK);
}


Item_subselect::trans_res
Item_in_subselect::row_value_transformer(Join *join)
{
  Select_Lex *select_lex= join->select_lex;
  uint32_t cols_num= left_expr->cols();

  if (select_lex->item_list.size() != left_expr->cols())
  {
    my_error(ER_OPERAND_COLUMNS, MYF(0), left_expr->cols());
    return(RES_ERROR);
  }

  /*
    Wrap the current IN predicate in an Item_in_optimizer. The actual
    substitution in the Item tree takes place in Item_subselect::fix_fields.
  */
  if (!substitution)
  {
    //first call for this unit
    Select_Lex_Unit *master_unit= select_lex->master_unit();
    substitution= optimizer;

    Select_Lex *current= session->lex().current_select, *up;
    session->lex().current_select= up= current->return_after_parsing();
    //optimizer never use Item **ref => we can pass 0 as parameter
    if (!optimizer || optimizer->fix_left(session, 0))
    {
      session->lex().current_select= current;
      return(RES_ERROR);
    }

    // we will refer to upper level cache array => we have to save it in PS
    optimizer->keep_top_level_cache();

    session->lex().current_select= current;
    master_unit->uncacheable.set(UNCACHEABLE_DEPENDENT);

    if (!abort_on_null && left_expr->maybe_null && !pushed_cond_guards)
    {
      if (!(pushed_cond_guards= (bool*)join->session->getMemRoot()->allocate(sizeof(bool) *
                                                        left_expr->cols())))
        return(RES_ERROR);
      for (uint32_t i= 0; i < cols_num; i++)
        pushed_cond_guards[i]= true;
    }
  }

  /*
    If this IN predicate can be computed via materialization, do not
    perform the IN -> EXISTS transformation.
  */
  if (exec_method == MATERIALIZATION)
    return(RES_OK);

  /* Perform the IN=>EXISTS transformation. */
  return(row_value_in_to_exists_transformer(join));
}


/**
  Tranform a (possibly non-correlated) IN subquery into a correlated EXISTS.

  @todo
  The IF-ELSE below can be refactored so that there is no duplication of the
  statements that create the new conditions. For this we have to invert the IF
  and the FOR statements as this:
  for (each left operand)
    create the equi-join condition
    if (is_having_used || !abort_on_null)
      create the "is null" and is_not_null_test items
    if (is_having_used)
      add the equi-join and the null tests to HAVING
    else
      add the equi-join and the "is null" to WHERE
      add the is_not_null_test to HAVING
*/

Item_subselect::trans_res
Item_in_subselect::row_value_in_to_exists_transformer(Join * join)
{
  Select_Lex *select_lex= join->select_lex;
  Item *having_item= 0;
  uint32_t cols_num= left_expr->cols();
  bool is_having_used= (join->having || select_lex->with_sum_func ||
                        select_lex->group_list.first ||
                        !select_lex->table_list.elements);

  select_lex->uncacheable.set(UNCACHEABLE_DEPENDENT);
  if (is_having_used)
  {
    /*
      (l1, l2, l3) IN (SELECT v1, v2, v3 ... HAVING having) =>
      EXISTS (SELECT ... HAVING having and
                                (l1 = v1 or is null v1) and
                                (l2 = v2 or is null v2) and
                                (l3 = v3 or is null v3) and
                                is_not_null_test(v1) and
                                is_not_null_test(v2) and
                                is_not_null_test(v3))
      where is_not_null_test used to register nulls in case if we have
      not found matching to return correct NULL value
      TODO: say here explicitly if the order of AND parts matters or not.
    */
    Item *item_having_part2= 0;
    for (uint32_t i= 0; i < cols_num; i++)
    {
      assert((left_expr->fixed && select_lex->ref_pointer_array[i]->fixed) ||
                  (select_lex->ref_pointer_array[i]->type() == REF_ITEM &&
                   ((Item_ref*)(select_lex->ref_pointer_array[i]))->ref_type() ==
                    Item_ref::OUTER_REF));
      if (select_lex->ref_pointer_array[i]->
          check_cols(left_expr->element_index(i)->cols()))
        return(RES_ERROR);
      Item *item_eq=
        new Item_func_eq(new
                         Item_ref(&select_lex->context,
                                  (*optimizer->get_cache())->
                                  addr(i),
                                  (char *)"<no matter>",
                                  (char *)in_left_expr_name),
                         new
                         Item_ref(&select_lex->context,
                                  select_lex->ref_pointer_array + i,
                                  (char *)"<no matter>",
                                  (char *)"<list ref>")
                        );
      Item *item_isnull=
        new Item_func_isnull(new
                             Item_ref(&select_lex->context,
                                      select_lex->ref_pointer_array+i,
                                      (char *)"<no matter>",
                                      (char *)"<list ref>")
                            );
      Item *col_item= new Item_cond_or(item_eq, item_isnull);
      if (!abort_on_null && left_expr->element_index(i)->maybe_null)
      {
        if (!(col_item= new Item_func_trig_cond(col_item, get_cond_guard(i))))
          return(RES_ERROR);
      }
      having_item= and_items(having_item, col_item);

      Item *item_nnull_test=
         new Item_is_not_null_test(this,
                                   new Item_ref(&select_lex->context,
                                                select_lex->
                                                ref_pointer_array + i,
                                                (char *)"<no matter>",
                                                (char *)"<list ref>"));
      if (!abort_on_null && left_expr->element_index(i)->maybe_null)
      {
        if (!(item_nnull_test=
              new Item_func_trig_cond(item_nnull_test, get_cond_guard(i))))
          return(RES_ERROR);
      }
      item_having_part2= and_items(item_having_part2, item_nnull_test);
      item_having_part2->top_level_item();
    }
    having_item= and_items(having_item, item_having_part2);
    having_item->top_level_item();
  }
  else
  {
    /*
      (l1, l2, l3) IN (SELECT v1, v2, v3 ... WHERE where) =>
      EXISTS (SELECT ... WHERE where and
                               (l1 = v1 or is null v1) and
                               (l2 = v2 or is null v2) and
                               (l3 = v3 or is null v3)
                         HAVING is_not_null_test(v1) and
                                is_not_null_test(v2) and
                                is_not_null_test(v3))
      where is_not_null_test register NULLs values but reject rows

      in case when we do not need correct NULL, we have simplier construction:
      EXISTS (SELECT ... WHERE where and
                               (l1 = v1) and
                               (l2 = v2) and
                               (l3 = v3)
    */
    Item *where_item= 0;
    for (uint32_t i= 0; i < cols_num; i++)
    {
      Item *item, *item_isnull;
      assert((left_expr->fixed && select_lex->ref_pointer_array[i]->fixed) ||
                  (select_lex->ref_pointer_array[i]->type() == REF_ITEM &&
                   ((Item_ref*)(select_lex->ref_pointer_array[i]))->ref_type() ==
                    Item_ref::OUTER_REF));
      if (select_lex->ref_pointer_array[i]->
          check_cols(left_expr->element_index(i)->cols()))
        return(RES_ERROR);
      item=
        new Item_func_eq(new
                         Item_direct_ref(&select_lex->context,
                                         (*optimizer->get_cache())->
                                         addr(i),
                                         (char *)"<no matter>",
                                         (char *)in_left_expr_name),
                         new
                         Item_direct_ref(&select_lex->context,
                                         select_lex->
                                         ref_pointer_array+i,
                                         (char *)"<no matter>",
                                         (char *)"<list ref>")
                        );
      if (!abort_on_null)
      {
        Item *having_col_item=
          new Item_is_not_null_test(this,
                                    new
                                    Item_ref(&select_lex->context,
                                             select_lex->ref_pointer_array + i,
                                             (char *)"<no matter>",
                                             (char *)"<list ref>"));


        item_isnull= new
          Item_func_isnull(new
                           Item_direct_ref(&select_lex->context,
                                           select_lex->
                                           ref_pointer_array+i,
                                           (char *)"<no matter>",
                                           (char *)"<list ref>")
                          );
        item= new Item_cond_or(item, item_isnull);
        /*
          TODO: why we create the above for cases where the right part
                cant be NULL?
        */
        if (left_expr->element_index(i)->maybe_null)
        {
          if (!(item= new Item_func_trig_cond(item, get_cond_guard(i))))
            return(RES_ERROR);
          if (!(having_col_item=
                  new Item_func_trig_cond(having_col_item, get_cond_guard(i))))
            return(RES_ERROR);
        }
        having_item= and_items(having_item, having_col_item);
      }
      where_item= and_items(where_item, item);
    }
    /*
      AND can't be changed during fix_fields()
      we can assign select_lex->where here, and pass 0 as last
      argument (reference) to fix_fields()
    */
    select_lex->where= join->conds= and_items(join->conds, where_item);
    select_lex->where->top_level_item();
    if (join->conds->fix_fields(session, 0))
      return(RES_ERROR);
  }
  if (having_item)
  {
    bool res;
    select_lex->having= join->having= and_items(join->having, having_item);
    if (having_item == select_lex->having)
      having_item->name= (char*)in_having_cond;
    select_lex->having->top_level_item();
    /*
      AND can't be changed during fix_fields()
      we can assign select_lex->having here, and pass 0 as last
      argument (reference) to fix_fields()
    */
    select_lex->having_fix_field= 1;
    res= join->having->fix_fields(session, 0);
    select_lex->having_fix_field= 0;
    if (res)
    {
      return(RES_ERROR);
    }
  }

  return(RES_OK);
}


Item_subselect::trans_res
Item_in_subselect::select_transformer(Join *join)
{
  return select_in_like_transformer(join, Eq_creator::instance());
}


/**
  Prepare IN/ALL/ANY/SOME subquery transformation and call appropriate
  transformation function.

    To decide which transformation procedure (scalar or row) applicable here
    we have to call fix_fields() for left expression to be able to call
    cols() method on it. Also this method make arena management for
    underlying transformation methods.

  @param join    JOIN object of transforming subquery
  @param func    creator of condition function of subquery

  @retval
    RES_OK      OK
  @retval
    RES_REDUCE  OK, and current subquery was reduced during
    transformation
  @retval
    RES_ERROR   Error
*/

Item_subselect::trans_res
Item_in_subselect::select_in_like_transformer(Join *join, const Comp_creator *func)
{
  Select_Lex *current= session->lex().current_select, *up;
  const char *save_where= session->where();
  Item_subselect::trans_res res= RES_ERROR;
  bool result;

  {
    /*
      IN/SOME/ALL/ANY subqueries aren't support LIMIT clause. Without it
      ORDER BY clause becomes meaningless thus we drop it here.
    */
    Select_Lex *sl= current->master_unit()->first_select();
    for (; sl; sl= sl->next_select())
    {
      if (sl->join)
        sl->join->order= 0;
    }
  }

  if (changed)
    return(RES_OK);

  session->setWhere("IN/ALL/ANY subquery");

  /*
    In some optimisation cases we will not need this Item_in_optimizer
    object, but we can't know it here, but here we need address correct
    reference on left expresion.
  */
  if (!optimizer)
  {
    result= (!(optimizer= new Item_in_optimizer(left_expr, this)));
    if (result)
      goto err;
  }

  session->lex().current_select= up= current->return_after_parsing();
  result= (!left_expr->fixed &&
           left_expr->fix_fields(session, optimizer->arguments()));
  /* fix_fields can change reference to left_expr, we need reassign it */
  left_expr= optimizer->arguments()[0];

  session->lex().current_select= current;
  if (result)
    goto err;

  /*
    If we didn't choose an execution method up to this point, we choose
    the IN=>EXISTS transformation.
  */
  if (exec_method == NOT_TRANSFORMED)
    exec_method= IN_TO_EXISTS;

  /*
    Both transformers call fix_fields() only for Items created inside them,
    and all those items do not make permanent changes in the current item arena
    which allows us to call them with changed arena (if we do not know the
    nature of Item, we have to call fix_fields() for it only with the original
    arena to avoid memory leak).
  */
  if (left_expr->cols() == 1)
    res= single_value_transformer(join, func);
  else
  {
    /* we do not support row operation for ALL/ANY/SOME */
    if (func != Eq_creator::instance())
    {
      my_error(ER_OPERAND_COLUMNS, MYF(0), 1);
      return(RES_ERROR);
    }
    res= row_value_transformer(join);
  }
err:
  session->setWhere(save_where);
  return(res);
}


void Item_in_subselect::print(String *str)
{
  if (exec_method == IN_TO_EXISTS)
    str->append(STRING_WITH_LEN("<exists>"));
  else
  {
    left_expr->print(str);
    str->append(STRING_WITH_LEN(" in "));
  }
  Item_subselect::print(str);
}


bool Item_in_subselect::fix_fields(Session *session_arg, Item **ref)
{
  bool result = 0;

  if (exec_method == SEMI_JOIN)
    return !( (*ref)= new Item_int(1));

  return result || Item_subselect::fix_fields(session_arg, ref);
}


/**
  Try to create an engine to compute the subselect via materialization,
  and if this fails, revert to execution via the IN=>EXISTS transformation.

  @details
    The purpose of this method is to hide the implementation details
    of this Item's execution. The method creates a new engine for
    materialized execution, and initializes the engine.

    If this initialization fails
    - either because it wasn't possible to create the needed temporary table
      and its index,
    - or because of a memory allocation error,
    then we revert back to execution via the IN=>EXISTS tranformation.

    The initialization of the new engine is divided in two parts - a permanent
    one that lives across prepared statements, and one that is repeated for each
    execution.

  @returns
    @retval true  memory allocation error occurred
    @retval false an execution method was chosen successfully
*/

bool Item_in_subselect::setup_engine()
{
  subselect_hash_sj_engine *new_engine= NULL;
  bool res= false;

  if (engine->engine_type() == subselect_engine::SINGLE_SELECT_ENGINE)
  {
    /* Create/initialize objects in permanent memory. */
    subselect_single_select_engine *old_engine_ptr;

    old_engine_ptr= static_cast<subselect_single_select_engine *>(engine);

    if (!(new_engine= new subselect_hash_sj_engine(session, this,
                                                   old_engine_ptr)) ||
        new_engine->init_permanent(unit->get_unit_column_types()))
    {
      Item_subselect::trans_res new_trans_res;
      /*
        If for some reason we cannot use materialization for this IN predicate,
        delete all materialization-related objects, and apply the IN=>EXISTS
        transformation.
      */
      delete new_engine;
      new_engine= NULL;
      exec_method= NOT_TRANSFORMED;
      if (left_expr->cols() == 1)
        new_trans_res= single_value_in_to_exists_transformer(
                           old_engine_ptr->join,
                           Eq_creator::instance());
      else
        new_trans_res= row_value_in_to_exists_transformer(old_engine_ptr->join);
      res= (new_trans_res != Item_subselect::RES_OK);
    }
    if (new_engine)
      engine= new_engine;
  }
  else
  {
    assert(engine->engine_type() == subselect_engine::HASH_SJ_ENGINE);
    new_engine= static_cast<subselect_hash_sj_engine *>(engine);
  }

  /* Initilizations done in runtime memory, repeated for each execution. */
  if (new_engine)
  {
    /*
      Reset the LIMIT 1 set in Item_exists_subselect::fix_length_and_dec.
      TODO:
      Currently we set the subquery LIMIT to infinity, and this is correct
      because we forbid at parse time LIMIT inside IN subqueries (see
      Item_in_subselect::test_limit). However, once we allow this, here
      we should set the correct limit if given in the query.
    */
    unit->global_parameters->select_limit= NULL;
    if ((res= new_engine->init_runtime()))
      return(res);
  }

  return(res);
}


/**
  Initialize the cache of the left operand of the IN predicate.

  @note This method has the same purpose as alloc_group_fields(),
  but it takes a different kind of collection of items, and the
  list we push to is dynamically allocated.

  @retval true  if a memory allocation error occurred or the cache is
                not applicable to the current query
  @retval false if success
*/

bool Item_in_subselect::init_left_expr_cache()
{
  Join *outer_join= NULL;

  outer_join= unit->outer_select()->join;
  if (! outer_join || ! outer_join->tables || ! outer_join->join_tab)
    return true;

  if (!(left_expr_cache= new List<Cached_item>))
    return true;

  for (uint32_t i= 0; i < left_expr->cols(); i++)
  {
    Cached_item *cur_item_cache= new_Cached_item(session, left_expr->element_index(i));
    if (!cur_item_cache || left_expr_cache->push_front(cur_item_cache))
      return true;
  }
  return false;
}


/*
  Callback to test if an IN predicate is expensive.

  @details
    IN predicates are considered expensive only if they will be executed via
    materialization. The return value affects the behavior of
    make_cond_for_table() in such a way that it is unchanged when we use
    the IN=>EXISTS transformation to compute IN.

  @retval true  if the predicate is expensive
  @retval false otherwise
*/

bool Item_in_subselect::is_expensive_processor(unsigned char *)
{
  return exec_method == MATERIALIZATION;
}


Item_subselect::trans_res
Item_allany_subselect::select_transformer(Join *join)
{
  exec_method= IN_TO_EXISTS;
  if (upper_item)
    upper_item->show= 1;
  return(select_in_like_transformer(join, func));
}


void Item_allany_subselect::print(String *str)
{
  if (exec_method == IN_TO_EXISTS)
    str->append(STRING_WITH_LEN("<exists>"));
  else
  {
    left_expr->print(str);
    str->append(' ');
    str->append(func->symbol(all));
    str->append(all ? " all " : " any ", 5);
  }
  Item_subselect::print(str);
}


void subselect_engine::set_session(Session *session_arg)
{
  session= session_arg;
  if (result)
    result->set_session(session_arg);
}


subselect_single_select_engine::
subselect_single_select_engine(Select_Lex *select,
			       select_result_interceptor *result_arg,
			       Item_subselect *item_arg)
  :subselect_engine(item_arg, result_arg),
   prepared(0), executed(0), select_lex(select), join(0)
{
  select_lex->master_unit()->item= item_arg;
}


void subselect_single_select_engine::cleanup()
{
  prepared= executed= 0;
  join= 0;
  result->cleanup();
  return;
}


void subselect_union_engine::cleanup()
{
  unit->reinit_exec_mechanism();
  result->cleanup();
  return;
}


bool subselect_union_engine::is_executed() const
{
  return unit->executed;
}


/*
  Check if last execution of the subquery engine produced any rows

  SYNOPSIS
    subselect_union_engine::no_rows()

  DESCRIPTION
    Check if last execution of the subquery engine produced any rows. The
    return value is undefined if last execution ended in an error.

  RETURN
    true  - Last subselect execution has produced no rows
    false - Otherwise
*/

bool subselect_union_engine::no_rows()
{
  /* Check if we got any rows when reading UNION result from temp. table: */
  return test(!unit->fake_select_lex->join->send_records);
}


void subselect_uniquesubquery_engine::cleanup()
{
  /* Tell handler we don't need the index anymore */
  if (tab->table->cursor->inited)
    tab->table->cursor->endIndexScan();
  return;
}


subselect_union_engine::subselect_union_engine(Select_Lex_Unit *u,
					       select_result_interceptor *result_arg,
					       Item_subselect *item_arg)
  :subselect_engine(item_arg, result_arg)
{
  unit= u;
  unit->item= item_arg;
}


/**
  Create and prepare the JOIN object that represents the query execution
  plan for the subquery.

  @detail
  This method is called from Item_subselect::fix_fields. For prepared
  statements it is called both during the PREPARE and EXECUTE phases in the
  following ways:
  - During PREPARE the optimizer needs some properties
    (join->fields_list.elements) of the JOIN to proceed with preparation of
    the remaining query (namely to complete ::fix_fields for the subselect
    related classes. In the end of PREPARE the JOIN is deleted.
  - When we EXECUTE the query, Item_subselect::fix_fields is called again, and
    the JOIN object is re-created again, prepared and executed. In the end of
    execution it is deleted.
  In all cases the JOIN is created in runtime memory (not in the permanent
  memory root).

  @todo
  Re-check what properties of 'join' are needed during prepare, and see if
  we can avoid creating a JOIN during JOIN::prepare of the outer join.

  @retval 0  if success
  @retval 1  if error
*/

int subselect_single_select_engine::prepare()
{
  if (prepared)
    return 0;
  join= new Join(session, select_lex->item_list,
		 select_lex->options | SELECT_NO_UNLOCK, result);
  if (!join || !result)
    return 1; /* Fatal error is set already. */
  prepared= 1;
  Select_Lex *save_select= session->lex().current_select;
  session->lex().current_select= select_lex;
  if (join->prepare(&select_lex->ref_pointer_array,
		    (TableList*) select_lex->table_list.first,
		    select_lex->with_wild,
		    select_lex->where,
		    select_lex->order_list.elements +
		    select_lex->group_list.elements,
		    (Order*) select_lex->order_list.first,
		    (Order*) select_lex->group_list.first,
		    select_lex->having,
		    select_lex, select_lex->master_unit()))
    return 1;
  session->lex().current_select= save_select;
  return 0;
}

int subselect_union_engine::prepare()
{
  return unit->prepare(session, result, (uint32_t)SELECT_NO_UNLOCK);
}

int subselect_uniquesubquery_engine::prepare()
{
  /* Should never be called. */
  assert(false);
  return 1;
}


/*
  Check if last execution of the subquery engine produced any rows

  SYNOPSIS
    subselect_single_select_engine::no_rows()

  DESCRIPTION
    Check if last execution of the subquery engine produced any rows. The
    return value is undefined if last execution ended in an error.

  RETURN
    true  - Last subselect execution has produced no rows
    false - Otherwise
*/

bool subselect_single_select_engine::no_rows()
{
  return !item->assigned();
}


/*
 makes storage for the output values for the subquery and calcuates
 their data and column types and their nullability.
*/
void subselect_engine::set_row(List<Item> &item_list, Item_cache **row)
{
  Item *sel_item;
  List<Item>::iterator li(item_list.begin());
  res_type= STRING_RESULT;
  res_field_type= DRIZZLE_TYPE_VARCHAR;
  for (uint32_t i= 0; (sel_item= li++); i++)
  {
    item->max_length= sel_item->max_length;
    res_type= sel_item->result_type();
    res_field_type= sel_item->field_type();
    item->decimals= sel_item->decimals;
    item->unsigned_flag= sel_item->unsigned_flag;
    maybe_null= sel_item->maybe_null;
    if (!(row[i]= Item_cache::get_cache(sel_item)))
      return;
    row[i]->setup(sel_item);
  }
  if (item_list.size() > 1)
    res_type= ROW_RESULT;
}

void subselect_single_select_engine::fix_length_and_dec(Item_cache **row)
{
  assert(row || select_lex->item_list.size() == 1);
  set_row(select_lex->item_list, row);
  item->collation.set(row[0]->collation);
  if (cols() != 1)
    maybe_null= 0;
}

void subselect_union_engine::fix_length_and_dec(Item_cache **row)
{
  assert(row || unit->first_select()->item_list.size() == 1);

  if (unit->first_select()->item_list.size() == 1)
  {
    set_row(unit->types, row);
    item->collation.set(row[0]->collation);
  }
  else
  {
    bool maybe_null_saved= maybe_null;
    set_row(unit->types, row);
    maybe_null= maybe_null_saved;
  }
}

void subselect_uniquesubquery_engine::fix_length_and_dec(Item_cache **)
{
  //this never should be called
  assert(0);
}

int subselect_single_select_engine::exec()
{
  char const *save_where= session->where();
  Select_Lex *save_select= session->lex().current_select;
  session->lex().current_select= select_lex;
  if (!join->optimized)
  {
    Select_Lex_Unit *unit= select_lex->master_unit();

    unit->set_limit(unit->global_parameters);
    if (join->optimize())
    {
      session->setWhere(save_where);
      executed= 1;
      session->lex().current_select= save_select;
      return(join->error ? join->error : 1);
    }
    if (save_join_if_explain())
     return(1);

    if (item->engine_changed)
    {
      return(1);
    }
  }
  if (select_lex->uncacheable.any() &&
      ! select_lex->uncacheable.test(UNCACHEABLE_EXPLAIN) &&
      executed)
  {
    if (join->reinit())
    {
      session->setWhere(save_where);
      session->lex().current_select= save_select;
      return 1;
    }
    item->reset();
    item->assigned((executed= 0));
  }
  if (!executed)
  {
    item->reset_value_registration();
    JoinTable *changed_tabs[MAX_TABLES];
    JoinTable **last_changed_tab= changed_tabs;
    if (item->have_guarded_conds())
    {
      /*
        For at least one of the pushed predicates the following is true:
        We should not apply optimizations based on the condition that was
        pushed down into the subquery. Those optimizations are ref[_or_null]
        acceses. Change them to be full table scans.
      */
      for (uint32_t i=join->const_tables ; i < join->tables ; i++)
      {
        JoinTable *tab=join->join_tab+i;
        if (tab && tab->keyuse)
        {
          for (uint32_t key_part= 0;
               key_part < tab->ref.key_parts;
               key_part++)
          {
            bool *cond_guard= tab->ref.cond_guards[key_part];
            if (cond_guard && !*cond_guard)
            {
              /* Change the access method to full table scan */
              tab->save_read_first_record= tab->read_first_record;
              tab->save_read_record= tab->read_record.read_record;
              tab->read_first_record= init_read_record_seq;
              tab->read_record.record= tab->table->record[0];
              tab->read_record.session= join->session;
              tab->read_record.ref_length= tab->table->cursor->ref_length;
              *(last_changed_tab++)= tab;
              break;
            }
          }
        }
      }
    }

    join->exec();

    /* Enable the optimizations back */
    for (JoinTable **ptab= changed_tabs; ptab != last_changed_tab; ptab++)
    {
      JoinTable *tab= *ptab;
      tab->read_record.record= 0;
      tab->read_record.ref_length= 0;
      tab->read_first_record= tab->save_read_first_record;
      tab->read_record.read_record= tab->save_read_record;
    }
    executed= 1;
    session->setWhere(save_where);
    session->lex().current_select= save_select;
    return(join->error||session->is_fatal_error);
  }
  session->setWhere(save_where);
  session->lex().current_select= save_select;
  return(0);
}

bool 
subselect_single_select_engine::save_join_if_explain()
{
  /*
    Save this JOIN to join->tmp_join since the original layout will be
    replaced when JOIN::exec() calls make_simple_join() if:
     1) We are executing an EXPLAIN query
     2) An uncacheable flag has not been set for the select_lex. If
        set, JOIN::optimize() has already saved the JOIN
     3) Call does not come from select_describe()). If it does,
        JOIN::exec() will not call make_simple_join() and the JOIN we
        plan to save will not be replaced anyway.
     4) A temp table is needed. This is what triggers JOIN::exec() to
        make a replacement JOIN by calling make_simple_join(). 
     5) The Item_subselect is cacheable
  */
  if (session->lex().describe &&                          // 1
      select_lex->uncacheable.none() &&                  // 2
      !(join->select_options & SELECT_DESCRIBE) &&       // 3
      join->need_tmp &&                                  // 4
      item->const_item())                                // 5
  {
    /*
      Save this JOIN to join->tmp_join since the original layout will
      be replaced when JOIN::exec() calls make_simple_join() due to
      need_tmp==TRUE. The original layout is needed so we can describe
      the query. No need to do this if uncacheable != 0 since in this
      case the JOIN has already been saved during JOIN::optimize()
    */
    select_lex->uncacheable.set(UNCACHEABLE_EXPLAIN);
    select_lex->master_unit()->uncacheable.set(UNCACHEABLE_EXPLAIN);
    if (join->init_save_join_tab())
      return true;
  }
  return false;
}


int subselect_union_engine::exec()
{
  char const *save_where= session->where();
  int res= unit->exec();
  session->setWhere(save_where);

  return res;
}


/*
  Search for at least one row satisfying select condition

  SYNOPSIS
    subselect_uniquesubquery_engine::scan_table()

  DESCRIPTION
    Scan the table using sequential access until we find at least one row
    satisfying select condition.

    The caller must set this->empty_result_set=false before calling this
    function. This function will set it to true if it finds a matching row.

  RETURN
    false - OK
    true  - Error
*/

int subselect_uniquesubquery_engine::scan_table()
{
  int error;
  Table *table= tab->table;

  if (table->cursor->inited)
    table->cursor->endIndexScan();

  if ((error= table->cursor->startTableScan(1)))
  {
    table->print_error(error, MYF(0));
    return 1;
  }

  assert(table->getSession());
  table->cursor->extra_opt(HA_EXTRA_CACHE,
                           table->getSession()->variables.read_buff_size);
  table->null_row= 0;
  for (;;)
  {
    error=table->cursor->rnd_next(table->record[0]);
    if (error && error != HA_ERR_END_OF_FILE)
    {
      error= table->report_error(error);
      break;
    }
    /* No more rows */
    if (table->status)
      break;

    if (!cond || cond->val_int())
    {
      empty_result_set= false;
      break;
    }
  }

  table->cursor->endTableScan();
  return(error != 0);
}


/*
  Copy ref key and check for null parts in it

  SYNOPSIS
    subselect_uniquesubquery_engine::copy_ref_key()

  DESCRIPTION
    Copy ref key and check for null parts in it.
    Depending on the nullability and conversion problems this function
    recognizes and processes the following states :
      1. Partial match on top level. This means IN has a value of false
         regardless of the data in the subquery table.
         Detected by finding a NULL in the left IN operand of a top level
         expression.
         We may actually skip reading the subquery, so return true to skip
         the table scan in subselect_uniquesubquery_engine::exec and make
         the value of the IN predicate a NULL (that is equal to false on
         top level).
      2. No exact match when IN is nested inside another predicate.
         Detected by finding a NULL in the left IN operand when IN is not
         a top level predicate.
         We cannot have an exact match. But we must proceed further with a
         table scan to find out if it's a partial match (and IN has a value
         of NULL) or no match (and IN has a value of false).
         So we return false to continue with the scan and see if there are
         any record that would constitute a partial match (as we cannot
         determine that from the index).
      3. Error converting the left IN operand to the column type of the
         right IN operand. This counts as no match (and IN has the value of
         false). We mark the subquery table cursor as having no more rows
         (to ensure that the processing that follows will not find a match)
         and return false, so IN is not treated as returning NULL.


  RETURN
    false - The value of the IN predicate is not known. Proceed to find the
            value of the IN predicate using the determined values of
            null_keypart and table->status.
    true  - IN predicate has a value of NULL. Stop the processing right there
            and return NULL to the outer predicates.
*/

bool subselect_uniquesubquery_engine::copy_ref_key()
{
  for (StoredKey **copy= tab->ref.key_copy ; *copy ; copy++)
  {
    StoredKey::store_key_result store_res= (*copy)->copy();
    tab->ref.key_err= store_res;

    /*
      When there is a NULL part in the key we don't need to make index
      lookup for such key thus we don't need to copy whole key.
      If we later should do a sequential scan return OK. Fail otherwise.

      See also the comment for the subselect_uniquesubquery_engine::exec()
      function.
    */
    null_keypart= (*copy)->null_key;
    if (null_keypart)
    {
      bool top_level= ((Item_in_subselect *) item)->is_top_level_item();
      if (top_level)
      {
        /* Partial match on top level */
        return(1);
      }
      else
      {
        /* No exact match when IN is nested inside another predicate */
        break;
      }
    }

    /*
      Check if the error is equal to STORE_KEY_FATAL. This is not expressed
      using the StoredKey::store_key_result enum because ref.key_err is a
      boolean and we want to detect both true and STORE_KEY_FATAL from the
      space of the union of the values of [true, false] and
      StoredKey::store_key_result.
      TODO: fix the variable an return types.
    */
    if (store_res == StoredKey::STORE_KEY_FATAL)
    {
      /*
       Error converting the left IN operand to the column type of the right
       IN operand.
      */
      tab->table->status= STATUS_NOT_FOUND;
      break;
    }
  }
  return(0);
}


/*
  Execute subselect

  SYNOPSIS
    subselect_uniquesubquery_engine::exec()

  DESCRIPTION
    Find rows corresponding to the ref key using index access.
    If some part of the lookup key is NULL, then we're evaluating
      NULL IN (SELECT ... )
    This is a special case, we don't need to search for NULL in the table,
    instead, the result value is
      - NULL  if select produces empty row set
      - false otherwise.

    In some cases (IN subselect is a top level item, i.e. abort_on_null==true)
    the caller doesn't distinguish between NULL and false result and we just
    return false.
    Otherwise we make a full table scan to see if there is at least one
    matching row.

    The result of this function (info about whether a row was found) is
    stored in this->empty_result_set.
  NOTE

  RETURN
    false - ok
    true  - an error occured while scanning
*/

int subselect_uniquesubquery_engine::exec()
{
  int error;
  Table *table= tab->table;
  empty_result_set= true;
  table->status= 0;

  /* TODO: change to use of 'full_scan' here? */
  if (copy_ref_key())
    return(1);
  if (table->status)
  {
    /*
      We know that there will be no rows even if we scan.
      Can be set in copy_ref_key.
    */
    ((Item_in_subselect *) item)->value= 0;
    return(0);
  }

  if (null_keypart)
    return(scan_table());

  if (!table->cursor->inited)
  {
    error= table->cursor->startIndexScan(tab->ref.key, 0);

    if (error != 0)
    {
      error= table->report_error(error);
      return (error != 0);
    }
  }

  error= table->cursor->index_read_map(table->record[0],
                                     tab->ref.key_buff,
                                     make_prev_keypart_map(tab->ref.key_parts),
                                     HA_READ_KEY_EXACT);
  if (error &&
      error != HA_ERR_KEY_NOT_FOUND && error != HA_ERR_END_OF_FILE)
    error= table->report_error(error);
  else
  {
    error= 0;
    table->null_row= 0;
    if (!table->status && (!cond || cond->val_int()))
    {
      ((Item_in_subselect *) item)->value= 1;
      empty_result_set= false;
    }
    else
      ((Item_in_subselect *) item)->value= 0;
  }

  return(error != 0);
}


/*
  Index-lookup subselect 'engine' - run the subquery

  SYNOPSIS
    subselect_indexsubquery_engine:exec()
      full_scan

  DESCRIPTION
    The engine is used to resolve subqueries in form

      oe IN (SELECT key FROM tbl WHERE subq_where)

    The value of the predicate is calculated as follows:
    1. If oe IS NULL, this is a special case, do a full table scan on
       table tbl and search for row that satisfies subq_where. If such
       row is found, return NULL, otherwise return false.
    2. Make an index lookup via key=oe, search for a row that satisfies
       subq_where. If found, return true.
    3. If check_null==true, make another lookup via key=NULL, search for a
       row that satisfies subq_where. If found, return NULL, otherwise
       return false.

  TODO
    The step #1 can be optimized further when the index has several key
    parts. Consider a subquery:

      (oe1, oe2) IN (SELECT keypart1, keypart2 FROM tbl WHERE subq_where)

    and suppose we need to evaluate it for {oe1, oe2}=={const1, NULL}.
    Current code will do a full table scan and obtain correct result. There
    is a better option: instead of evaluating

      SELECT keypart1, keypart2 FROM tbl WHERE subq_where            (1)

    and checking if it has produced any matching rows, evaluate

      SELECT keypart2 FROM tbl WHERE subq_where AND keypart1=const1  (2)

    If this query produces a row, the result is NULL (as we're evaluating
    "(const1, NULL) IN { (const1, X), ... }", which has a value of UNKNOWN,
    i.e. NULL).  If the query produces no rows, the result is false.

    We currently evaluate (1) by doing a full table scan. (2) can be
    evaluated by doing a "ref" scan on "keypart1=const1", which can be much
    cheaper. We can use index statistics to quickly check whether "ref" scan
    will be cheaper than full table scan.

  RETURN
    0
    1
*/

int subselect_indexsubquery_engine::exec()
{
  int error;
  bool null_finding= 0;
  Table *table= tab->table;

  ((Item_in_subselect *) item)->value= 0;
  empty_result_set= true;
  null_keypart= 0;
  table->status= 0;

  if (check_null)
  {
    /* We need to check for NULL if there wasn't a matching value */
    *tab->ref.null_ref_key= 0;			// Search first for not null
    ((Item_in_subselect *) item)->was_null= 0;
  }

  /* Copy the ref key and check for nulls... */
  if (copy_ref_key())
    return(1);

  if (table->status)
  {
    /*
      We know that there will be no rows even if we scan.
      Can be set in copy_ref_key.
    */
    ((Item_in_subselect *) item)->value= 0;
    return(0);
  }

  if (null_keypart)
    return(scan_table());

  if (!table->cursor->inited)
  {
    error= table->cursor->startIndexScan(tab->ref.key, 1);

    if (error != 0)
    {
      error= table->report_error(error);
      return(error != 0);
    }
  }
  error= table->cursor->index_read_map(table->record[0],
                                     tab->ref.key_buff,
                                     make_prev_keypart_map(tab->ref.key_parts),
                                     HA_READ_KEY_EXACT);
  if (error &&
      error != HA_ERR_KEY_NOT_FOUND && error != HA_ERR_END_OF_FILE)
    error= table->report_error(error);
  else
  {
    for (;;)
    {
      error= 0;
      table->null_row= 0;
      if (!table->status)
      {
        if ((!cond || cond->val_int()) && (!having || having->val_int()))
        {
          empty_result_set= false;
          if (null_finding)
            ((Item_in_subselect *) item)->was_null= 1;
          else
            ((Item_in_subselect *) item)->value= 1;
          break;
        }
        error= table->cursor->index_next_same(table->record[0],
                                            tab->ref.key_buff,
                                            tab->ref.key_length);
        if (error && error != HA_ERR_END_OF_FILE)
        {
          error= table->report_error(error);
          break;
        }
      }
      else
      {
        if (!check_null || null_finding)
          break;			/* We don't need to check nulls */
        *tab->ref.null_ref_key= 1;
        null_finding= 1;
        /* Check if there exists a row with a null value in the index */
        if ((error= (safe_index_read(tab) == 1)))
          break;
      }
    }
  }
  return(error != 0);
}


uint32_t subselect_single_select_engine::cols()
{
  return select_lex->item_list.size();
}


uint32_t subselect_union_engine::cols()
{
  return unit->types.size();
}


bool subselect_single_select_engine::uncacheable()
{
  return select_lex->uncacheable.any();
}


bool subselect_single_select_engine::uncacheable(uint32_t bit_pos)
{
  return select_lex->uncacheable.test(bit_pos);
}


bool subselect_union_engine::uncacheable()
{
  return unit->uncacheable.any();
}


bool subselect_union_engine::uncacheable(uint32_t bit_pos)
{
  return unit->uncacheable.test(bit_pos);
}


void subselect_single_select_engine::exclude()
{
  select_lex->master_unit()->exclude_level();
}

void subselect_union_engine::exclude()
{
  unit->exclude_level();
}


void subselect_uniquesubquery_engine::exclude()
{
  //this never should be called
  assert(0);
}


table_map subselect_engine::calc_const_tables(TableList *table)
{
  table_map map= 0;
  for (; table; table= table->next_leaf)
  {
    Table *tbl= table->table;
    if (tbl && tbl->const_table)
      map|= tbl->map;
  }
  return map;
}


table_map subselect_single_select_engine::upper_select_const_tables()
{
  return calc_const_tables((TableList *) select_lex->outer_select()->
			   leaf_tables);
}


table_map subselect_union_engine::upper_select_const_tables()
{
  return calc_const_tables((TableList *) unit->outer_select()->leaf_tables);
}


void subselect_single_select_engine::print(String *str)
{
  select_lex->print(session, str);
}


void subselect_union_engine::print(String *str)
{
  unit->print(str);
}


void subselect_uniquesubquery_engine::print(String *str)
{
  const char *table_name= tab->table->getShare()->getTableName();
  str->append(STRING_WITH_LEN("<primary_index_lookup>("));
  tab->ref.items[0]->print(str);
  str->append(STRING_WITH_LEN(" in "));
  if (tab->table->getShare()->isTemporaryCategory())
  {
    /*
      Temporary tables' names change across runs, so they can't be used for
      EXPLAIN EXTENDED.
    */
    str->append(STRING_WITH_LEN("<temporary table>"));
  }
  else
    str->append(table_name, tab->table->getShare()->getTableNameSize());
  KeyInfo *key_info= tab->table->key_info+ tab->ref.key;
  str->append(STRING_WITH_LEN(" on "));
  str->append(key_info->name);
  if (cond)
  {
    str->append(STRING_WITH_LEN(" where "));
    cond->print(str);
  }
  str->append(')');
}

/*
TODO:
The above ::print method should be changed as below. Do it after
all other tests pass.

void subselect_uniquesubquery_engine::print(String *str)
{
  KEY *key_info= tab->table->key_info + tab->ref.key;
  str->append(STRING_WITH_LEN("<primary_index_lookup>("));
  for (uint32_t i= 0; i < key_info->key_parts; i++)
    tab->ref.items[i]->print(str);
  str->append(STRING_WITH_LEN(" in "));
  str->append(tab->table->getShare()->getTableName(), tab->table->getShare()->getTableNameSize());
  str->append(STRING_WITH_LEN(" on "));
  str->append(key_info->name);
  if (cond)
  {
    str->append(STRING_WITH_LEN(" where "));
    cond->print(str);
  }
  str->append(')');
}
*/

void subselect_indexsubquery_engine::print(String *str)
{
  str->append(STRING_WITH_LEN("<index_lookup>("));
  tab->ref.items[0]->print(str);
  str->append(STRING_WITH_LEN(" in "));
  str->append(tab->table->getShare()->getTableName(), tab->table->getShare()->getTableNameSize());
  KeyInfo *key_info= tab->table->key_info+ tab->ref.key;
  str->append(STRING_WITH_LEN(" on "));
  str->append(key_info->name);
  if (check_null)
    str->append(STRING_WITH_LEN(" checking NULL"));
  if (cond)
  {
    str->append(STRING_WITH_LEN(" where "));
    cond->print(str);
  }
  if (having)
  {
    str->append(STRING_WITH_LEN(" having "));
    having->print(str);
  }
  str->append(')');
}

/**
  change select_result object of engine.

  @param si		new subselect Item
  @param res		new select_result object

  @retval
    false OK
  @retval
    true  error
*/

bool subselect_single_select_engine::change_result(Item_subselect *si,
                                                 select_result_interceptor *res)
{
  item= si;
  result= res;
  return select_lex->join->change_result(result);
}


/**
  change select_result object of engine.

  @param si		new subselect Item
  @param res		new select_result object

  @retval
    false OK
  @retval
    true  error
*/

bool subselect_union_engine::change_result(Item_subselect *si,
                                           select_result_interceptor *res)
{
  item= si;
  int rc= unit->change_result(res, result);
  result= res;
  return rc;
}


/**
  change select_result emulation, never should be called.

  @param si		new subselect Item
  @param res		new select_result object

  @retval
    false OK
  @retval
    true  error
*/

bool subselect_uniquesubquery_engine::change_result(Item_subselect *,
                                                    select_result_interceptor *)
{
  assert(0);
  return true;
}


/**
  Report about presence of tables in subquery.

  @retval
    true  there are not tables used in subquery
  @retval
    false there are some tables in subquery
*/
bool subselect_single_select_engine::no_tables()
{
  return(select_lex->table_list.elements == 0);
}


/*
  Check statically whether the subquery can return NULL

  SINOPSYS
    subselect_single_select_engine::may_be_null()

  RETURN
    false  can guarantee that the subquery never return NULL
    true   otherwise
*/
bool subselect_single_select_engine::may_be_null()
{
  return ((no_tables() && !join->conds && !join->having) ? maybe_null : 1);
}


/**
  Report about presence of tables in subquery.

  @retval
    true  there are not tables used in subquery
  @retval
    false there are some tables in subquery
*/
bool subselect_union_engine::no_tables()
{
  for (Select_Lex *sl= unit->first_select(); sl; sl= sl->next_select())
  {
    if (sl->table_list.elements)
      return false;
  }
  return true;
}


/**
  Report about presence of tables in subquery.

  @retval
    true  there are not tables used in subquery
  @retval
    false there are some tables in subquery
*/

bool subselect_uniquesubquery_engine::no_tables()
{
  /* returning value is correct, but this method should never be called */
  return 0;
}


/******************************************************************************
  WL#1110 - Implementation of class subselect_hash_sj_engine
******************************************************************************/


/**
  Create all structures needed for IN execution that can live between PS
  reexecution.

  @detail
  - Create a temporary table to store the result of the IN subquery. The
    temporary table has one hash index on all its columns.
  - Create a new result sink that sends the result stream of the subquery to
    the temporary table,
  - Create and initialize a new JoinTable, and TABLE_REF objects to perform
    lookups into the indexed temporary table.

  @notice:
    Currently Item_subselect::init() already chooses and creates at parse
    time an engine with a corresponding JOIN to execute the subquery.

  @retval true  if error
  @retval false otherwise
*/

bool subselect_hash_sj_engine::init_permanent(List<Item> *tmp_columns)
{
  /* The result sink where we will materialize the subquery result. */
  select_union  *tmp_result_sink;
  /* The table into which the subquery is materialized. */
  Table         *tmp_table;
  KeyInfo           *tmp_key; /* The only index on the temporary table. */
  uint32_t          tmp_key_parts; /* Number of keyparts in tmp_key. */
  Item_in_subselect *item_in= (Item_in_subselect *) item;

  /* 1. Create/initialize materialization related objects. */

  /*
    Create and initialize a select result interceptor that stores the
    result stream in a temporary table. The temporary table itself is
    managed (created/filled/etc) internally by the interceptor.
  */
  if (!(tmp_result_sink= new select_union))
    return(true);

  if (tmp_result_sink->create_result_table(
                         session, tmp_columns, true,
                         session->options | TMP_TABLE_ALL_COLUMNS,
                         "materialized subselect"))
    return(true);

  tmp_table= tmp_result_sink->table;
  tmp_key= tmp_table->key_info;
  tmp_key_parts= tmp_key->key_parts;

  /*
     If the subquery has blobs, or the total key lenght is bigger than some
     length, then the created index cannot be used for lookups and we
     can't use hash semi join. If this is the case, delete the temporary
     table since it will not be used, and tell the caller we failed to
     initialize the engine.
  */
  if (tmp_table->getShare()->sizeKeys() == 0)
  {
    assert(tmp_table->getShare()->db_type() == myisam_engine);
    assert(
      tmp_table->getShare()->uniques ||
      tmp_table->key_info->key_length >= tmp_table->cursor->getEngine()->max_key_length() ||
      tmp_table->key_info->key_parts > tmp_table->cursor->getEngine()->max_key_parts());
    tmp_table= NULL;
    delete result;
    result= NULL;
    return(true);
  }
  result= tmp_result_sink;

  /*
    Make sure there is only one index on the temp table, and it doesn't have
    the extra key part created when s->uniques > 0.
  */
  assert(tmp_table->getShare()->sizeKeys() == 1 && tmp_columns->size() == tmp_key_parts);


  /* 2. Create/initialize execution related objects. */

  /*
    Create and initialize the JoinTable that represents an index lookup
    plan operator into the materialized subquery result. Notice that:
    - this JoinTable has no corresponding JOIN (and doesn't need one), and
    - here we initialize only those members that are used by
      subselect_uniquesubquery_engine, so these objects are incomplete.
  */
  if (!(tab= (JoinTable*) session->getMemRoot()->allocate(sizeof(JoinTable))))
    return(true);
  new (tab) JoinTable();
  tab->table= tmp_table;
  tab->ref.key= 0; /* The only temp table index. */
  tab->ref.key_length= tmp_key->key_length;
  if (!(tab->ref.key_buff=
        (unsigned char*) session->calloc(ALIGN_SIZE(tmp_key->key_length) * 2)) ||
      !(tab->ref.key_copy=
        (StoredKey**) session->getMemRoot()->allocate((sizeof(StoredKey*) *
                                  (tmp_key_parts + 1)))) ||
      !(tab->ref.items=
        (Item**) session->getMemRoot()->allocate(sizeof(Item*) * tmp_key_parts)))
    return(true);

  KeyPartInfo *cur_key_part= tmp_key->key_part;
  StoredKey **ref_key= tab->ref.key_copy;
  unsigned char *cur_ref_buff= tab->ref.key_buff;

  for (uint32_t i= 0; i < tmp_key_parts; i++, cur_key_part++, ref_key++)
  {
    tab->ref.items[i]= item_in->left_expr->element_index(i);
    int null_count= test(cur_key_part->field->real_maybe_null());
    *ref_key= new store_key_item(session, cur_key_part->field,
                                 /* TODO:
                                    the NULL byte is taken into account in
                                    cur_key_part->store_length, so instead of
                                    cur_ref_buff + test(maybe_null), we could
                                    use that information instead.
                                 */
                                 cur_ref_buff + null_count,
                                 null_count ? tab->ref.key_buff : 0,
                                 cur_key_part->length, tab->ref.items[i]);
    cur_ref_buff+= cur_key_part->store_length;
  }
  *ref_key= NULL; /* End marker. */
  tab->ref.key_err= 1;
  tab->ref.key_parts= tmp_key_parts;

  return(false);
}


/**
  Initialize members of the engine that need to be re-initilized at each
  execution.

  @retval true  if a memory allocation error occurred
  @retval false if success
*/

bool subselect_hash_sj_engine::init_runtime()
{
  /*
    Create and optimize the JOIN that will be used to materialize
    the subquery if not yet created.
  */
  materialize_engine->prepare();
  /* Let our engine reuse this query plan for materialization. */
  materialize_join= materialize_engine->join;
  materialize_join->change_result(result);
  return false;
}


subselect_hash_sj_engine::~subselect_hash_sj_engine()
{
  delete result;
  if (tab)
  {
    tab->table= NULL;
  }
}


/**
  Cleanup performed after each PS execution.

  @detail
  Called in the end of Join::prepare for PS from Item_subselect::cleanup.
*/

void subselect_hash_sj_engine::cleanup()
{
  is_materialized= false;
  result->cleanup(); /* Resets the temp table as well. */
  materialize_engine->cleanup();
  subselect_uniquesubquery_engine::cleanup();
}


/**
  Execute a subquery IN predicate via materialization.

  @detail
  If needed materialize the subquery into a temporary table, then
  copmpute the predicate via a lookup into this table.

  @retval true  if error
  @retval false otherwise
*/

int subselect_hash_sj_engine::exec()
{
  Item_in_subselect *item_in= (Item_in_subselect *) item;

  /*
    Optimize and materialize the subquery during the first execution of
    the subquery predicate.
  */
  if (!is_materialized)
  {
    int res= 0;
    Select_Lex *save_select= session->lex().current_select;
    session->lex().current_select= materialize_engine->select_lex;
    if ((res= materialize_join->optimize()))
      goto err;

    if (materialize_engine->save_join_if_explain())
      goto err;

    materialize_join->exec();
    if ((res= test(materialize_join->error || session->is_fatal_error)))
      goto err;

    /*
      TODO:
      - Unlock all subquery tables as we don't need them. To implement this
        we need to add new functionality to Join::join_free that can unlock
        all tables in a subquery (and all its subqueries).
      - The temp table used for grouping in the subquery can be freed
        immediately after materialization (yet it's done together with
        unlocking).
     */
    is_materialized= true;
    /*
      If the subquery returned no rows, the temporary table is empty, so we know
      directly that the result of IN is false. We first update the table
      statistics, then we test if the temporary table for the query result is
      empty.
    */
    tab->table->cursor->info(HA_STATUS_VARIABLE);
    if (!tab->table->cursor->stats.records)
    {
      empty_result_set= true;
      item_in->value= false;
      /* TODO: check we need this: item_in->null_value= false; */
      return(false);
    }
    /* Set tmp_param only if its usable, i.e. tmp_param->copy_field != NULL. */
    tmp_param= &(item_in->unit->outer_select()->join->tmp_table_param);
    if (tmp_param && !tmp_param->copy_field)
      tmp_param= NULL;

err:
    session->lex().current_select= save_select;
    if (res)
      return(res);
  }

  /*
    Lookup the left IN operand in the hash index of the materialized subquery.
  */
  return(subselect_uniquesubquery_engine::exec());
}


/**
  Print the state of this engine into a string for debugging and views.
*/

void subselect_hash_sj_engine::print(String *str)
{
  str->append(STRING_WITH_LEN(" <materialize> ("));
  materialize_engine->print(str);
  str->append(STRING_WITH_LEN(" ), "));
  if (tab)
    subselect_uniquesubquery_engine::print(str);
  else
    str->append(STRING_WITH_LEN(
           "<the access method for lookups is not yet created>"
         ));
}

} /* namespace drizzled */
