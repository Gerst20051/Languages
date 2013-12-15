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

#include <config.h>

#include <drizzled/function/str/collation.h>

namespace drizzled
{

String *Item_func_collation::val_str(String *str)
{
  assert(fixed == 1);
  size_t dummy_errors;
  const CHARSET_INFO * const cs= args[0]->collation.collation;

  null_value= 0;
  str->copy(cs->name, strlen(cs->name),
            &my_charset_utf8_general_ci, collation.collation, &dummy_errors);
  return str;
}

} /* namespace drizzled */
