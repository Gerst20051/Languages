/* - mode: c; c-basic-offset: 2; indent-tabs-mode: nil; -*-
 *  vim:expandtab:shiftwidth=2:tabstop=2:smarttab:
 *
 *  Copyright (C) 2010 Brian Aker
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

#include <config.h>
#include <plugin/information_schema_dictionary/dictionary.h>

using namespace std;
using namespace drizzled;

RoutineColumns::RoutineColumns() :
  InformationSchema("ROUTINE_COLUMNS")
{
  add_field("TABLE_CATALOG");
  add_field("TABLE_SCHEMA");
  add_field("TABLE_NAME");

  add_field("COLUMN_NAME");
  add_field("ORDINAL_POSITION");
  add_field("COLUMN_DEFAULT");

  add_field("IS_NULLABLE");

  add_field("DATA_TYPE");
  add_field("CHARACTER_MAXIMUM_LENGTH");
  add_field("CHARACTER_OCTET_LENGTH");
  add_field("NUMERIC_PRECISION");
  add_field("NUMERIC_PRECISION_RADIX");
  add_field("NUMERIC_SCALE");
  add_field("DATETIME_PRECISION");
  add_field("CHARACTER_SET_CATALOG");
  add_field("CHARACTER_SET_SCHEMA");
  add_field("CHARACTER_SET_NAME");
  add_field("COLLATION_CATALOG");
  add_field("COLLATION_SCHEMA");
  add_field("COLLATION_NAME");
  add_field("DOMAIN_CATALOG");
  add_field("DOMAIN_SCHEMA");
  add_field("DOMAIN_NAME");
}

void RoutineColumns::Generator::fill()
{
}

bool RoutineColumns::Generator::nextCore()
{
  return false;
}

bool RoutineColumns::Generator::next()
{
  while (not nextCore())
  {
    return false;
  }

  return true;
}

RoutineColumns::Generator::Generator(drizzled::Field **arg) :
  InformationSchema::Generator(arg),
  is_primed(false)
{
}

bool RoutineColumns::Generator::populate()
{
  if (not next())
    return false;

  fill();

  return true;
}
