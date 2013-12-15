/* -*- mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; -*-
 *  vim:expandtab:shiftwidth=2:tabstop=2:smarttab:
 *
 *  Copyright (C) 2008 Sun Microsystems, Inc.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
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

#pragma once

/* classes for sum functions */

#include <drizzled/tree.h>
#include <drizzled/hybrid_type.h>
#include <drizzled/item.h>
#include <drizzled/item/field.h>
#include <drizzled/item/bin_string.h>
#include <drizzled/charset_info.h>

namespace drizzled
{

int group_concat_key_cmp_with_distinct(void* arg, const void* key1,
                                       const void* key2);

int group_concat_key_cmp_with_order(void* arg, const void* key1,
                                    const void* key2);

class Select_Lex;
struct Order;

/*
  Class Item_sum is the base class used for special expressions that SQL calls
  'set functions'. These expressions are formed with the help of aggregate
  functions such as SUM, MAX, GROUP_CONCAT etc.

 GENERAL NOTES

  A set function cannot be used in certain positions where expressions are
  accepted. There are some quite explicable restrictions for the usage of
  set functions.

  In the query:
    SELECT AVG(b) FROM t1 WHERE SUM(b) > 20 GROUP by a
  the usage of the set function AVG(b) is legal, while the usage of SUM(b)
  is illegal. A WHERE condition must contain expressions that can be
  evaluated for each row of the table. Yet the expression SUM(b) can be
  evaluated only for each group of rows with the same value of column a.
  In the query:
    SELECT AVG(b) FROM t1 WHERE c > 30 GROUP BY a HAVING SUM(b) > 20
  both set function expressions AVG(b) and SUM(b) are legal.

  We can say that in a query without nested selects an occurrence of a
  set function in an expression of the SELECT list or/and in the HAVING
  clause is legal, while in the WHERE clause it's illegal.

  The general rule to detect whether a set function is legal in a query with
  nested subqueries is much more complicated.

  Consider the the following query:
    SELECT t1.a FROM t1 GROUP BY t1.a
      HAVING t1.a > ALL (SELECT t2.c FROM t2 WHERE SUM(t1.b) < t2.c).
  The set function SUM(b) is used here in the WHERE clause of the subquery.
  Nevertheless it is legal since it is under the HAVING clause of the query
  to which this function relates. The expression SUM(t1.b) is evaluated
  for each group defined in the main query, not for groups of the subquery.

  The problem of finding the query where to aggregate a particular
  set function is not so simple as it seems to be.

  In the query:
    SELECT t1.a FROM t1 GROUP BY t1.a
     HAVING t1.a > ALL(SELECT t2.c FROM t2 GROUP BY t2.c
                         HAVING SUM(t1.a) < t2.c)
  the set function can be evaluated for both outer and inner selects.
  If we evaluate SUM(t1.a) for the outer query then we get the value of t1.a
  multiplied by the cardinality of a group in table t1. In this case
  in each correlated subquery SUM(t1.a) is used as a constant. But we also
  can evaluate SUM(t1.a) for the inner query. In this case t1.a will be a
  constant for each correlated subquery and summation is performed
  for each group of table t2.
  (Here it makes sense to remind that the query
    SELECT c FROM t GROUP BY a HAVING SUM(1) < a
  is quite legal in our SQL).

  So depending on what query we assign the set function to we
  can get different result sets.

  The general rule to detect the query where a set function is to be
  evaluated can be formulated as follows.
  Consider a set function S(E) where E is an expression with occurrences
  of column references C1, ..., CN. Resolve these column references against
  subqueries that contain the set function S(E). Let Q be the innermost
  subquery of those subqueries. (It should be noted here that S(E)
  in no way can be evaluated in the subquery embedding the subquery Q,
  otherwise S(E) would refer to at least one unbound column reference)
  If S(E) is used in a construct of Q where set functions are allowed then
  we evaluate S(E) in Q.
  Otherwise we look for a innermost subquery containing S(E) of those where
  usage of S(E) is allowed.

  Let's demonstrate how this rule is applied to the following queries.

  1. SELECT t1.a FROM t1 GROUP BY t1.a
       HAVING t1.a > ALL(SELECT t2.b FROM t2 GROUP BY t2.b
                           HAVING t2.b > ALL(SELECT t3.c FROM t3 GROUP BY t3.c
                                                HAVING SUM(t1.a+t2.b) < t3.c))
  For this query the set function SUM(t1.a+t2.b) depends on t1.a and t2.b
  with t1.a defined in the outermost query, and t2.b defined for its
  subquery. The set function is in the HAVING clause of the subquery and can
  be evaluated in this subquery.

  2. SELECT t1.a FROM t1 GROUP BY t1.a
       HAVING t1.a > ALL(SELECT t2.b FROM t2
                           WHERE t2.b > ALL (SELECT t3.c FROM t3 GROUP BY t3.c
                                               HAVING SUM(t1.a+t2.b) < t3.c))
  Here the set function SUM(t1.a+t2.b)is in the WHERE clause of the second
  subquery - the most upper subquery where t1.a and t2.b are defined.
  If we evaluate the function in this subquery we violate the context rules.
  So we evaluate the function in the third subquery (over table t3) where it
  is used under the HAVING clause.

  3. SELECT t1.a FROM t1 GROUP BY t1.a
       HAVING t1.a > ALL(SELECT t2.b FROM t2
                           WHERE t2.b > ALL (SELECT t3.c FROM t3
                                               WHERE SUM(t1.a+t2.b) < t3.c))
  In this query evaluation of SUM(t1.a+t2.b) is not legal neither in the second
  nor in the third subqueries. So this query is invalid.

  Mostly set functions cannot be nested. In the query
    SELECT t1.a from t1 GROUP BY t1.a HAVING AVG(SUM(t1.b)) > 20
  the expression SUM(b) is not acceptable, though it is under a HAVING clause.
  Yet it is acceptable in the query:
    SELECT t.1 FROM t1 GROUP BY t1.a HAVING SUM(t1.b) > 20.

  An argument of a set function does not have to be a reference to a table
  column as we saw it in examples above. This can be a more complex expression
    SELECT t1.a FROM t1 GROUP BY t1.a HAVING SUM(t1.b+1) > 20.
  The expression SUM(t1.b+1) has a very clear semantics in this context:
  we sum up the values of t1.b+1 where t1.b varies for all values within a
  group of rows that contain the same t1.a value.

  A set function for an outer query yields a constant within a subquery. So
  the semantics of the query
    SELECT t1.a FROM t1 GROUP BY t1.a
      HAVING t1.a IN (SELECT t2.c FROM t2 GROUP BY t2.c
                        HAVING AVG(t2.c+SUM(t1.b)) > 20)
  is still clear. For a group of the rows with the same t1.a values we
  calculate the value of SUM(t1.b). This value 's' is substituted in the
  the subquery:
    SELECT t2.c FROM t2 GROUP BY t2.c HAVING AVG(t2.c+s)
  than returns some result set.

  By the same reason the following query with a subquery
    SELECT t1.a FROM t1 GROUP BY t1.a
      HAVING t1.a IN (SELECT t2.c FROM t2 GROUP BY t2.c
                        HAVING AVG(SUM(t1.b)) > 20)
  is also acceptable.

 IMPLEMENTATION NOTES

  Three methods were added to the class to check the constraints specified
  in the previous section. These methods utilize several new members.

  The field 'nest_level' contains the number of the level for the subquery
  containing the set function. The main SELECT is of level 0, its subqueries
  are of levels 1, the subqueries of the latter are of level 2 and so on.

  The field 'aggr_level' is to contain the nest level of the subquery
  where the set function is aggregated.

  The field 'max_arg_level' is for the maximun of the nest levels of the
  unbound column references occurred in the set function. A column reference
  is unbound  within a set function if it is not bound by any subquery
  used as a subexpression in this function. A column reference is bound by
  a subquery if it is a reference to the column by which the aggregation
  of some set function that is used in the subquery is calculated.
  For the set function used in the query
    SELECT t1.a FROM t1 GROUP BY t1.a
      HAVING t1.a > ALL(SELECT t2.b FROM t2 GROUP BY t2.b
                          HAVING t2.b > ALL(SELECT t3.c FROM t3 GROUP BY t3.c
                                              HAVING SUM(t1.a+t2.b) < t3.c))
  the value of max_arg_level is equal to 1 since t1.a is bound in the main
  query, and t2.b is bound by the first subquery whose nest level is 1.
  Obviously a set function cannot be aggregated in the subquery whose
  nest level is less than max_arg_level. (Yet it can be aggregated in the
  subqueries whose nest level is greater than max_arg_level.)
  In the query
    SELECT t.a FROM t1 HAVING AVG(t1.a+(SELECT MIN(t2.c) FROM t2))
  the value of the max_arg_level for the AVG set function is 0 since
  the reference t2.c is bound in the subquery.

  The field 'max_sum_func_level' is to contain the maximum of the
  nest levels of the set functions that are used as subexpressions of
  the arguments of the given set function, but not aggregated in any
  subquery within this set function. A nested set function s1 can be
  used within set function s0 only if s1.max_sum_func_level <
  s0.max_sum_func_level. Set function s1 is considered as nested
  for set function s0 if s1 is not calculated in any subquery
  within s0.

  A set function that is used as a subexpression in an argument of another
  set function refers to the latter via the field 'in_sum_func'.

  The condition imposed on the usage of set functions are checked when
  we traverse query subexpressions with the help of the recursive method
  fix_fields. When we apply this method to an object of the class
  Item_sum, first, on the descent, we call the method init_sum_func_check
  that initialize members used at checking. Then, on the ascent, we
  call the method check_sum_func that validates the set function usage
  and reports an error if it is illegal.
  The method register_sum_func serves to link the items for the set functions
  that are aggregated in the embedding (sub)queries. Circular chains of such
  functions are attached to the corresponding Select_Lex structures
  through the field inner_sum_func_list.

  Exploiting the fact that the members mentioned above are used in one
  recursive function we could have allocated them on the thread stack.
  Yet we don't do it now.

  We assume that the nesting level of subquries does not exceed 127.
  TODO: to catch queries where the limit is exceeded to make the
  code clean here.

*/

class Item_sum :public Item_result_field
{
public:
  enum Sumfunctype
  { COUNT_FUNC, COUNT_DISTINCT_FUNC, SUM_FUNC, SUM_DISTINCT_FUNC, AVG_FUNC,
    AVG_DISTINCT_FUNC, MIN_FUNC, MAX_FUNC, STD_FUNC,
    VARIANCE_FUNC, SUM_BIT_FUNC, GROUP_CONCAT_FUNC
  };

  Item **args, *tmp_args[2];
  Item **ref_by; /* pointer to a ref to the object used to register it */
  Item_sum *next; /* next in the circular chain of registered objects  */
  uint32_t arg_count;
  Item_sum *in_sum_func;  /* embedding set function if any */
  Select_Lex * aggr_sel; /* select where the function is aggregated       */
  int8_t nest_level;        /* number of the nesting level of the set function */
  int8_t aggr_level;        /* nesting level of the aggregating subquery       */
  int8_t max_arg_level;     /* max level of unbound column references          */
  int8_t max_sum_func_level;/* max level of aggregation for embedded functions */
  bool quick_group;			/* If incremental update of fields */
  /*
    This list is used by the check for mixing non aggregated fields and
    sum functions in the ONLY_FULL_GROUP_BY_MODE. We save all outer fields
    directly or indirectly used under this function it as it's unclear
    at the moment of fixing outer field whether it's aggregated or not.
  */
  List<Item_field> outer_fields;

protected:
  table_map used_tables_cache;
  bool forced_const;

public:

  void mark_as_sum_func();
  Item_sum() :arg_count(0), quick_group(1), forced_const(false)
  {
    mark_as_sum_func();
  }
  Item_sum(Item *a) :args(tmp_args), arg_count(1), quick_group(1),
    forced_const(false)
  {
    args[0]=a;
    mark_as_sum_func();
  }
  Item_sum( Item *a, Item *b ) :args(tmp_args), arg_count(2), quick_group(1),
    forced_const(false)
  {
    args[0]=a; args[1]=b;
    mark_as_sum_func();
  }
  Item_sum(List<Item> &list);
  //Copy constructor, need to perform subselects with temporary tables
  Item_sum(Session *session, Item_sum *item);
  enum Type type() const { return SUM_FUNC_ITEM; }
  virtual enum Sumfunctype sum_func () const=0;

  /*
    This method is similar to add(), but it is called when the current
    aggregation group changes. Thus it performs a combination of
    clear() and add().
  */
  inline bool reset() { clear(); return add(); };

  /*
    Prepare this item for evaluation of an aggregate value. This is
    called by reset() when a group changes, or, for correlated
    subqueries, between subquery executions.  E.g. for COUNT(), this
    method should set count= 0;
  */
  virtual void clear()= 0;

  /*
    This method is called for the next row in the same group. Its
    purpose is to aggregate the new value to the previous values in
    the group (i.e. since clear() was called last time). For example,
    for COUNT(), do count++.
  */
  virtual bool add()=0;

  /*
    Called when new group is started and results are being saved in
    a temporary table. Similar to reset(), but must also store value in
    result_field. Like reset() it is supposed to reset start value to
    default.
    This set of methods (reult_field(), reset_field, update_field()) of
    Item_sum is used only if quick_group is not null. Otherwise
    copy_or_same() is used to obtain a copy of this item.
  */
  virtual void reset_field()=0;
  /*
    Called for each new value in the group, when temporary table is in use.
    Similar to add(), but uses temporary table field to obtain current value,
    Updated value is then saved in the field.
  */
  virtual void update_field()=0;
  virtual bool keep_field_type(void) const { return 0; }
  virtual void fix_length_and_dec() { maybe_null=1; null_value=1; }
  /*
    This method is used for debug purposes to print the name of an
    item to the debug log. The second use of this method is as
    a helper function of print(), where it is applicable.
    To suit both goals it should return a meaningful,
    distinguishable and sintactically correct string.  This method
    should not be used for runtime type identification, use enum
    {Sum}Functype and Item_func::functype()/Item_sum::sum_func()
    instead.

    NOTE: for Items inherited from Item_sum, func_name() return part of
    function name till first argument (including '(') to make difference in
    names for functions with 'distinct' clause and without 'distinct' and
    also to make printing of items inherited from Item_sum uniform.
  */
  virtual const char *func_name() const= 0;
  virtual Item *result_item(Field *field)
    { return new Item_field(field); }
  table_map used_tables() const { return used_tables_cache; }
  void update_used_tables ();
  void cleanup()
  {
    Item::cleanup();
    forced_const= false;
  }
  bool is_null() { return null_value; }
  void make_const ()
  {
    used_tables_cache= 0;
    forced_const= true;
  }
  virtual bool const_item() const { return forced_const; }
  virtual bool const_during_execution() const { return false; }
  void make_field(SendField *field);
  virtual void print(String *str);
  void fix_num_length_and_dec();

  /*
    This function is called by the execution engine to assign 'NO ROWS
    FOUND' value to an aggregate item, when the underlying result set
    has no rows. Such value, in a general case, may be different from
    the default value of the item after 'clear()': e.g. a numeric item
    may be initialized to 0 by clear() and to NULL by
    no_rows_in_result().
  */
  void no_rows_in_result() { clear(); }

  virtual bool setup(Session *) {return 0;}
  virtual void make_unique(void) {}
  Item *get_tmp_table_item(Session *session);
  virtual Field *create_tmp_field(bool group, Table *table,
                                  uint32_t convert_blob_length);
  bool walk(Item_processor processor, bool walk_subquery, unsigned char *argument);
  bool init_sum_func_check(Session *session);
  bool check_sum_func(Session *session, Item **ref);
  bool register_sum_func(Session *session, Item **ref);
  Select_Lex *depended_from()
    { return (nest_level == aggr_level ? 0 : aggr_sel); }
};


class Item_sum_num :public Item_sum
{
protected:
  /*
   val_xxx() functions may be called several times during the execution of a
   query. Derived classes that require extensive calculation in val_xxx()
   maintain cache of aggregate value. This variable governs the validity of
   that cache.
  */
  bool is_evaluated;
public:
  Item_sum_num() :Item_sum(),is_evaluated(false) {}
  Item_sum_num(Item *item_par)
    :Item_sum(item_par), is_evaluated(false) {}
  Item_sum_num(Item *a, Item* b) :Item_sum(a,b),is_evaluated(false) {}
  Item_sum_num(List<Item> &list)
    :Item_sum(list), is_evaluated(false) {}
  Item_sum_num(Session *session, Item_sum_num *item)
    :Item_sum(session, item),is_evaluated(item->is_evaluated) {}
  bool fix_fields(Session *, Item **);
  int64_t val_int();
  String *val_str(String*str);
  type::Decimal *val_decimal(type::Decimal *);
  void reset_field();
};


class Item_sum_int :public Item_sum_num
{
public:
  Item_sum_int(Item *item_par) :Item_sum_num(item_par) {}
  Item_sum_int(List<Item> &list) :Item_sum_num(list) {}
  Item_sum_int(Session *session, Item_sum_int *item) :Item_sum_num(session, item) {}
  double val_real() { assert(fixed == 1); return (double) val_int(); }
  String *val_str(String*str);
  type::Decimal *val_decimal(type::Decimal *);
  enum Item_result result_type () const { return INT_RESULT; }
  void fix_length_and_dec()
  { decimals=0; max_length=21; maybe_null=null_value=0; }
};


class Item_sum_sum :public Item_sum_num
{
protected:
  Item_result hybrid_type;
  double sum;
  type::Decimal dec_buffs[2];
  uint32_t curr_dec_buff;
  void fix_length_and_dec();

public:
  Item_sum_sum(Item *item_par) :Item_sum_num(item_par) {}
  Item_sum_sum(Session *session, Item_sum_sum *item);
  enum Sumfunctype sum_func () const {return SUM_FUNC;}
  void clear();
  bool add();
  double val_real();
  int64_t val_int();
  String *val_str(String*str);
  type::Decimal *val_decimal(type::Decimal *);
  enum Item_result result_type () const { return hybrid_type; }
  void reset_field();
  void update_field();
  void no_rows_in_result() {}
  const char *func_name() const { return "sum("; }
  Item *copy_or_same(Session* session);
};



/* Common class for SUM(DISTINCT), AVG(DISTINCT) */

class Unique;

class Item_sum_distinct :public Item_sum_num
{
protected:
  /* storage for the summation result */
  uint64_t count;
  Hybrid_type val;
  /* storage for unique elements */
  Unique *tree;
  Table *table;
  enum enum_field_types table_field_type;
  uint32_t tree_key_length;
protected:
  Item_sum_distinct(Session *session, Item_sum_distinct *item);
public:
  Item_sum_distinct(Item *item_par);
  ~Item_sum_distinct();

  bool setup(Session *session);
  void clear();
  void cleanup();
  bool add();
  double val_real();
  type::Decimal *val_decimal(type::Decimal *);
  int64_t val_int();
  String *val_str(String *str);

  /* XXX: does it need make_unique? */

  enum Sumfunctype sum_func () const { return SUM_DISTINCT_FUNC; }
  void reset_field() {} // not used
  void update_field() {} // not used
  virtual void no_rows_in_result() {}
  void fix_length_and_dec();
  enum Item_result result_type () const;
  virtual void calculate_val_and_count();
  virtual bool unique_walk_function(void *elem);
};


/*
  Item_sum_sum_distinct - implementation of SUM(DISTINCT expr).
  See also: MySQL manual, chapter 'Adding New Functions To MySQL'
  and comments in item_sum.cc.
*/

class Item_sum_sum_distinct :public Item_sum_distinct
{
private:
  Item_sum_sum_distinct(Session *session, Item_sum_sum_distinct *item)
    :Item_sum_distinct(session, item) {}
public:
  Item_sum_sum_distinct(Item *item_arg) :Item_sum_distinct(item_arg) {}

  enum Sumfunctype sum_func () const { return SUM_DISTINCT_FUNC; }
  const char *func_name() const { return "sum(distinct "; }
  Item *copy_or_same(Session* session) { return new Item_sum_sum_distinct(session, this); }
};


/* Item_sum_avg_distinct - SELECT AVG(DISTINCT expr) FROM ... */

class Item_sum_avg_distinct: public Item_sum_distinct
{
private:
  Item_sum_avg_distinct(Session *session, Item_sum_avg_distinct *original)
    :Item_sum_distinct(session, original) {}
public:
  uint32_t prec_increment;
  Item_sum_avg_distinct(Item *item_arg) : Item_sum_distinct(item_arg) {}

  void fix_length_and_dec();
  virtual void calculate_val_and_count();
  enum Sumfunctype sum_func () const { return AVG_DISTINCT_FUNC; }
  const char *func_name() const { return "avg(distinct "; }
  Item *copy_or_same(Session* session) { return new Item_sum_avg_distinct(session, this); }
};


class Item_sum_count :public Item_sum_int
{
  int64_t count;

  public:
  Item_sum_count(Item *item_par)
    :Item_sum_int(item_par),count(0)
  {}
  Item_sum_count(Session *session, Item_sum_count *item)
    :Item_sum_int(session, item), count(item->count)
  {}
  enum Sumfunctype sum_func () const { return COUNT_FUNC; }
  void clear();
  void no_rows_in_result() { count=0; }
  bool add();
  void make_const_count(int64_t count_arg)
  {
    count=count_arg;
    Item_sum::make_const();
  }
  int64_t val_int();
  void reset_field();
  void cleanup();
  void update_field();
  const char *func_name() const { return "count("; }
  Item *copy_or_same(Session* session);
};


class Tmp_Table_Param;

class Item_sum_count_distinct :public Item_sum_int
{
  Table *table;
  uint32_t *field_lengths;
  Tmp_Table_Param *tmp_table_param;
  bool force_copy_fields;
  /*
    If there are no blobs, we can use a tree, which
    is faster than heap table. In that case, we still use the table
    to help get things set up, but we insert nothing in it
  */
  Unique *tree;
  /*
   Storage for the value of count between calls to val_int() so val_int()
   will not recalculate on each call. Validitiy of the value is stored in
   is_evaluated.
  */
  int64_t count;
  /*
    Following is 0 normal object and pointer to original one for copy
    (to correctly free resources)
  */
  Item_sum_count_distinct *original;
  uint32_t tree_key_length;


  bool always_null;		// Set to 1 if the result is always NULL


  friend int composite_key_cmp(void* arg, unsigned char* key1, unsigned char* key2);
  friend int simple_str_key_cmp(void* arg, unsigned char* key1, unsigned char* key2);

public:
  Item_sum_count_distinct(List<Item> &list)
    :Item_sum_int(list), table(0), field_lengths(0), tmp_table_param(0),
     force_copy_fields(0), tree(0), count(0),
     original(0), always_null(false)
  { quick_group= 0; }
  Item_sum_count_distinct(Session *session, Item_sum_count_distinct *item)
    :Item_sum_int(session, item), table(item->table),
     field_lengths(item->field_lengths),
     tmp_table_param(item->tmp_table_param),
     force_copy_fields(0), tree(item->tree), count(item->count),
     original(item), tree_key_length(item->tree_key_length),
     always_null(item->always_null)
  {}
  ~Item_sum_count_distinct();

  void cleanup();

  enum Sumfunctype sum_func () const { return COUNT_DISTINCT_FUNC; }
  void clear();
  bool add();
  int64_t val_int();
  void reset_field() { return ;}		// Never called
  void update_field() { return ; }		// Never called
  const char *func_name() const { return "count(distinct "; }
  bool setup(Session *session);
  void make_unique();
  Item *copy_or_same(Session* session);
  void no_rows_in_result() {}
};


/* Item to get the value of a stored sum function */

class Item_sum_avg;

class Item_avg_field :public Item_result_field
{
public:
  Field *field;
  Item_result hybrid_type;
  uint32_t f_precision, f_scale, dec_bin_size;
  uint32_t prec_increment;
  Item_avg_field(Item_result res_type, Item_sum_avg *item);
  enum Type type() const { return FIELD_AVG_ITEM; }
  double val_real();
  int64_t val_int();
  type::Decimal *val_decimal(type::Decimal *);
  bool is_null() { update_null_value(); return null_value; }
  String *val_str(String*);
  enum_field_types field_type() const
  {
    return hybrid_type == DECIMAL_RESULT ?
      DRIZZLE_TYPE_DECIMAL : DRIZZLE_TYPE_DOUBLE;
  }
  void fix_length_and_dec() {}
  enum Item_result result_type () const { return hybrid_type; }
};


class Item_sum_avg :public Item_sum_sum
{
public:
  uint64_t count;
  uint32_t prec_increment;
  uint32_t f_precision, f_scale, dec_bin_size;

  Item_sum_avg(Item *item_par) :Item_sum_sum(item_par), count(0) {}
  Item_sum_avg(Session *session, Item_sum_avg *item)
    :Item_sum_sum(session, item), count(item->count),
    prec_increment(item->prec_increment) {}

  void fix_length_and_dec();
  enum Sumfunctype sum_func () const {return AVG_FUNC;}
  void clear();
  bool add();
  double val_real();
  // In SPs we might force the "wrong" type with select into a declare variable
  int64_t val_int();
  type::Decimal *val_decimal(type::Decimal *);
  String *val_str(String *str);
  void reset_field();
  void update_field();
  Item *result_item(Field *)
  { return new Item_avg_field(hybrid_type, this); }
  void no_rows_in_result() {}
  const char *func_name() const { return "avg("; }
  Item *copy_or_same(Session* session);
  Field *create_tmp_field(bool group, Table *table, uint32_t convert_blob_length);
  void cleanup()
  {
    count= 0;
    Item_sum_sum::cleanup();
  }
};

class Item_sum_variance;

class Item_variance_field :public Item_result_field
{
public:
  Field *field;
  Item_result hybrid_type;
  uint32_t f_precision0, f_scale0;
  uint32_t f_precision1, f_scale1;
  uint32_t dec_bin_size0, dec_bin_size1;
  uint32_t sample;
  uint32_t prec_increment;
  Item_variance_field(Item_sum_variance *item);
  enum Type type() const {return FIELD_VARIANCE_ITEM; }
  double val_real();
  int64_t val_int();
  String *val_str(String *str)
  { return val_string_from_real(str); }
  type::Decimal *val_decimal(type::Decimal *dec_buf)
  { return val_decimal_from_real(dec_buf); }
  bool is_null() { update_null_value(); return null_value; }
  enum_field_types field_type() const
  {
    return hybrid_type == DECIMAL_RESULT ?
      DRIZZLE_TYPE_DECIMAL : DRIZZLE_TYPE_DOUBLE;
  }
  void fix_length_and_dec() {}
  enum Item_result result_type () const { return hybrid_type; }
};


/*
  variance(a) =

  =  sum (ai - avg(a))^2 / count(a) )
  =  sum (ai^2 - 2*ai*avg(a) + avg(a)^2) / count(a)
  =  (sum(ai^2) - sum(2*ai*avg(a)) + sum(avg(a)^2))/count(a) =
  =  (sum(ai^2) - 2*avg(a)*sum(a) + count(a)*avg(a)^2)/count(a) =
  =  (sum(ai^2) - 2*sum(a)*sum(a)/count(a) + count(a)*sum(a)^2/count(a)^2 )/count(a) =
  =  (sum(ai^2) - 2*sum(a)^2/count(a) + sum(a)^2/count(a) )/count(a) =
  =  (sum(ai^2) - sum(a)^2/count(a))/count(a)

But, this falls prey to catastrophic cancellation.  Instead, use the recurrence formulas

  M_{1} = x_{1}, ~ M_{k} = M_{k-1} + (x_{k} - M_{k-1}) / k newline
  S_{1} = 0, ~ S_{k} = S_{k-1} + (x_{k} - M_{k-1}) times (x_{k} - M_{k}) newline
  for 2 <= k <= n newline
  ital variance = S_{n} / (n-1)

*/

class Item_sum_variance : public Item_sum_num
{
  void fix_length_and_dec();

public:
  Item_result hybrid_type;
  int cur_dec;
  double recurrence_m, recurrence_s;    /* Used in recurrence relation. */
  uint64_t count;
  uint32_t f_precision0, f_scale0;
  uint32_t f_precision1, f_scale1;
  uint32_t dec_bin_size0, dec_bin_size1;
  uint32_t sample;
  uint32_t prec_increment;

  Item_sum_variance(Item *item_par, uint32_t sample_arg) :Item_sum_num(item_par),
    hybrid_type(REAL_RESULT), count(0), sample(sample_arg)
    {}
  Item_sum_variance(Session *session, Item_sum_variance *item);
  enum Sumfunctype sum_func () const { return VARIANCE_FUNC; }
  void clear();
  bool add();
  double val_real();
  int64_t val_int();
  type::Decimal *val_decimal(type::Decimal *);
  void reset_field();
  void update_field();
  Item *result_item(Field *)
  { return new Item_variance_field(this); }
  void no_rows_in_result() {}
  const char *func_name() const
    { return sample ? "var_samp(" : "variance("; }
  Item *copy_or_same(Session* session);
  Field *create_tmp_field(bool group, Table *table, uint32_t convert_blob_length);
  enum Item_result result_type () const { return REAL_RESULT; }
  void cleanup()
  {
    count= 0;
    Item_sum_num::cleanup();
  }
};

class Item_sum_std;

class Item_std_field :public Item_variance_field
{
public:
  Item_std_field(Item_sum_std *item);
  enum Type type() const { return FIELD_STD_ITEM; }
  double val_real();
  type::Decimal *val_decimal(type::Decimal *);
  enum Item_result result_type () const { return REAL_RESULT; }
  enum_field_types field_type() const { return DRIZZLE_TYPE_DOUBLE;}
};

/*
   standard_deviation(a) = sqrt(variance(a))
*/

class Item_sum_std :public Item_sum_variance
{
  public:
  Item_sum_std(Item *item_par, uint32_t sample_arg)
    :Item_sum_variance(item_par, sample_arg) {}
  Item_sum_std(Session *session, Item_sum_std *item)
    :Item_sum_variance(session, item)
    {}
  enum Sumfunctype sum_func () const { return STD_FUNC; }
  double val_real();
  Item *result_item(Field *)
    { return new Item_std_field(this); }
  const char *func_name() const { return "std("; }
  Item *copy_or_same(Session* session);
  enum Item_result result_type () const { return REAL_RESULT; }
  enum_field_types field_type() const { return DRIZZLE_TYPE_DOUBLE;}
};

// This class is a string or number function depending on num_func

class Item_sum_hybrid :public Item_sum
{
protected:
  String value,tmp_value;
  double sum;
  int64_t sum_int;
  type::Decimal sum_dec;
  Item_result hybrid_type;
  enum_field_types hybrid_field_type;
  int cmp_sign;
  bool was_values;  // Set if we have found at least one row (for max/min only)

  public:
  Item_sum_hybrid(Item *item_par,int sign)
    :Item_sum(item_par), sum(0.0), sum_int(0),
    hybrid_type(INT_RESULT), hybrid_field_type(DRIZZLE_TYPE_LONGLONG),
    cmp_sign(sign), was_values(true)
  { collation.set(&my_charset_bin); }
  Item_sum_hybrid(Session *session, Item_sum_hybrid *item);
  bool fix_fields(Session *, Item **);
  void clear();
  double val_real();
  int64_t val_int();
  type::Decimal *val_decimal(type::Decimal *);
  void reset_field();
  String *val_str(String *);
  bool keep_field_type(void) const { return 1; }
  enum Item_result result_type () const { return hybrid_type; }
  enum enum_field_types field_type() const { return hybrid_field_type; }
  void update_field();
  void min_max_update_str_field();
  void min_max_update_real_field();
  void min_max_update_int_field();
  void min_max_update_decimal_field();
  void cleanup();
  bool any_value() { return was_values; }
  void no_rows_in_result();
  Field *create_tmp_field(bool group, Table *table,
			  uint32_t convert_blob_length);
};


class Item_sum_min :public Item_sum_hybrid
{
public:
  Item_sum_min(Item *item_par) :Item_sum_hybrid(item_par,1) {}
  Item_sum_min(Session *session, Item_sum_min *item) :Item_sum_hybrid(session, item) {}
  enum Sumfunctype sum_func () const {return MIN_FUNC;}

  bool add();
  const char *func_name() const { return "min("; }
  Item *copy_or_same(Session* session);
};


class Item_sum_max :public Item_sum_hybrid
{
public:
  Item_sum_max(Item *item_par) :Item_sum_hybrid(item_par,-1) {}
  Item_sum_max(Session *session, Item_sum_max *item) :Item_sum_hybrid(session, item) {}
  enum Sumfunctype sum_func () const {return MAX_FUNC;}

  bool add();
  const char *func_name() const { return "max("; }
  Item *copy_or_same(Session* session);
};


class Item_sum_bit :public Item_sum_int
{
protected:
  uint64_t reset_bits,bits;

public:
  Item_sum_bit(Item *item_par,uint64_t reset_arg)
    :Item_sum_int(item_par),reset_bits(reset_arg),bits(reset_arg) {}
  Item_sum_bit(Session *session, Item_sum_bit *item):
    Item_sum_int(session, item), reset_bits(item->reset_bits), bits(item->bits) {}
  enum Sumfunctype sum_func () const {return SUM_BIT_FUNC;}
  void clear();
  int64_t val_int();
  void reset_field();
  void update_field();
  void fix_length_and_dec()
  { decimals= 0; max_length=21; unsigned_flag= 1; maybe_null= null_value= 0; }
  void cleanup()
  {
    bits= reset_bits;
    Item_sum_int::cleanup();
  }
};


class Item_sum_or :public Item_sum_bit
{
public:
  Item_sum_or(Item *item_par) :Item_sum_bit(item_par,0) {}
  Item_sum_or(Session *session, Item_sum_or *item) :Item_sum_bit(session, item) {}
  bool add();
  const char *func_name() const { return "bit_or("; }
  Item *copy_or_same(Session* session);
};


class Item_sum_and :public Item_sum_bit
{
  public:
  Item_sum_and(Item *item_par) :Item_sum_bit(item_par, UINT64_MAX) {}
  Item_sum_and(Session *session, Item_sum_and *item) :Item_sum_bit(session, item) {}
  bool add();
  const char *func_name() const { return "bit_and("; }
  Item *copy_or_same(Session* session);
};

class Item_sum_xor :public Item_sum_bit
{
  public:
  Item_sum_xor(Item *item_par) :Item_sum_bit(item_par,0) {}
  Item_sum_xor(Session *session, Item_sum_xor *item) :Item_sum_bit(session, item) {}
  bool add();
  const char *func_name() const { return "bit_xor("; }
  Item *copy_or_same(Session* session);
};



class DRIZZLE_ERROR;

class Item_func_group_concat : public Item_sum
{
  Tmp_Table_Param *tmp_table_param;
  DRIZZLE_ERROR *warning;
  String result;
  String *separator;
  TREE tree_base;
  TREE *tree;

  /**
     If DISTINCT is used with this GROUP_CONCAT, this member is used to filter
     out duplicates.
     @see Item_func_group_concat::setup
     @see Item_func_group_concat::add
     @see Item_func_group_concat::clear
   */
  Unique *unique_filter;
  Table *table;
  Order **order;
  Name_resolution_context *context;
  /** The number of ORDER BY items. */
  uint32_t arg_count_order;
  /** The number of selected items, aka the expr list. */
  uint32_t arg_count_field;
  uint32_t count_cut_values;
  bool distinct;
  bool warning_for_row;
  bool always_null;
  bool force_copy_fields;
  bool no_appended;
  /*
    Following is 0 normal object and pointer to original one for copy
    (to correctly free resources)
  */
  Item_func_group_concat *original;

  friend int group_concat_key_cmp_with_distinct(void* arg, const void* key1,
                                                const void* key2);
  friend int group_concat_key_cmp_with_order(void* arg, const void* key1,
					     const void* key2);
  friend int dump_leaf_key(unsigned char* key, uint32_t,
                           Item_func_group_concat *group_concat_item);

public:
  Item_func_group_concat(Name_resolution_context *context_arg,
                         bool is_distinct, List<Item> *is_select,
                         SQL_LIST *is_order, String *is_separator);

  Item_func_group_concat(Session *session, Item_func_group_concat *item);
  ~Item_func_group_concat();
  void cleanup();

  enum Sumfunctype sum_func () const {return GROUP_CONCAT_FUNC;}
  const char *func_name() const { return "group_concat"; }
  virtual Item_result result_type () const { return STRING_RESULT; }
  enum_field_types field_type() const
  {
    if (max_length/collation.collation->mbmaxlen > CONVERT_IF_BIGGER_TO_BLOB )
      return DRIZZLE_TYPE_BLOB;
    else
      return DRIZZLE_TYPE_VARCHAR;
  }
  void clear();
  bool add();
  void reset_field() { assert(0); }        // not used
  void update_field() { assert(0); }       // not used
  bool fix_fields(Session *,Item **);
  bool setup(Session *session);
  void make_unique();
  double val_real();
  int64_t val_int();
  type::Decimal *val_decimal(type::Decimal *decimal_value)
  {
    return val_decimal_from_string(decimal_value);
  }
  String* val_str(String* str);
  Item *copy_or_same(Session* session);
  void no_rows_in_result() {}
  virtual void print(String *str);
  virtual bool change_context_processor(unsigned char *cntx)
    { context= (Name_resolution_context *)cntx; return false; }
};

} /* namespace drizzled */

