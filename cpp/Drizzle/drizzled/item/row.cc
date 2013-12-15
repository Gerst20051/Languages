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

#include <config.h>
#include <drizzled/error.h>
#include <drizzled/session.h>

#include <drizzled/item/row.h>

namespace drizzled
{

/**
  Row items used for comparing rows and IN operations on rows:

  @verbatim
  (a, b, c) > (10, 10, 30)
  (a, b, c) = (select c, d, e, from t1 where x=12)
  (a, b, c) IN ((1,2,2), (3,4,5), (6,7,8)
  (a, b, c) IN (select c, d, e, from t1)
  @endverbatim

  @todo
    think placing 2-3 component items in item (as it done for function
*/

Item_row::Item_row(List<Item> &arg):
  Item(), used_tables_cache(0), const_item_cache(1), with_null(0)
{

  //TODO: think placing 2-3 component items in item (as it done for function)
  if ((arg_count= arg.size()))
    items= (Item**) memory::sql_alloc(sizeof(Item*)*arg_count);
  else
    items= 0;
  List<Item>::iterator li(arg.begin());
  uint32_t i= 0;
  Item *item;
  while ((item= li++))
  {
    items[i]= item;
    i++;
  }
}

void Item_row::illegal_method_call(const char *)
{
  assert(0);
  my_error(ER_OPERAND_COLUMNS, MYF(0), 1);
  return;
}

bool Item_row::fix_fields(Session *session, Item **)
{
  assert(fixed == 0);
  null_value= 0;
  maybe_null= 0;
  Item **arg, **arg_end;
  for (arg= items, arg_end= items+arg_count; arg != arg_end ; arg++)
  {
    if ((*arg)->fix_fields(session, arg))
      return true;
    // we can't assign 'item' before, because fix_fields() can change arg
    Item *item= *arg;
    used_tables_cache |= item->used_tables();
    const_item_cache&= item->const_item() && !with_null;
    if (const_item_cache)
    {
      if (item->cols() > 1)
	with_null|= item->null_inside();
      else
      {
	if (item->is_null())
          with_null|= 1;
      }
    }
    maybe_null|= item->maybe_null;
    with_sum_func= with_sum_func || item->with_sum_func;
  }
  fixed= 1;
  return false;
}


void Item_row::cleanup()
{
  Item::cleanup();
  /* Reset to the original values */
  used_tables_cache= 0;
  const_item_cache= true;
  with_null= 0;

  return;
}


void Item_row::split_sum_func(Session *session, Item **ref_pointer_array,
                              List<Item> &fields)
{
  Item **arg, **arg_end;
  for (arg= items, arg_end= items+arg_count; arg != arg_end ; arg++)
    (*arg)->split_sum_func(session, ref_pointer_array, fields, arg, true);
}


void Item_row::update_used_tables()
{
  used_tables_cache= 0;
  const_item_cache= true;
  for (uint32_t i= 0; i < arg_count; i++)
  {
    items[i]->update_used_tables();
    used_tables_cache|= items[i]->used_tables();
    const_item_cache&= items[i]->const_item();
  }
}

void Item_row::fix_after_pullout(Select_Lex *new_parent, Item **)
{
  used_tables_cache= 0;
  const_item_cache= true;
  for (uint32_t i= 0; i < arg_count; i++)
  {
    items[i]->fix_after_pullout(new_parent, &items[i]);
    used_tables_cache|= items[i]->used_tables();
    const_item_cache&= items[i]->const_item();
  }
}

bool Item_row::check_cols(uint32_t c)
{
  if (c != arg_count)
  {
    my_error(ER_OPERAND_COLUMNS, MYF(0), c);
    return 1;
  }
  return 0;
}

void Item_row::print(String *str)
{
  str->append('(');
  for (uint32_t i= 0; i < arg_count; i++)
  {
    if (i)
      str->append(',');
    items[i]->print(str);
  }
  str->append(')');
}


bool Item_row::walk(Item_processor processor, bool walk_subquery, unsigned char *arg)
{
  for (uint32_t i= 0; i < arg_count; i++)
  {
    if (items[i]->walk(processor, walk_subquery, arg))
      return 1;
  }
  return (this->*processor)(arg);
}


Item *Item_row::transform(Item_transformer transformer, unsigned char *arg)
{
  for (uint32_t i= 0; i < arg_count; i++)
  {
    Item *new_item= items[i]->transform(transformer, arg);
    if (!new_item)
      return 0;
      items[i]= new_item;
  }
  return (this->*transformer)(arg);
}

void Item_row::bring_value()
{
  for (uint32_t i= 0; i < arg_count; i++)
    items[i]->bring_value();
}

} /* namespace drizzled */
