/* - mode: c++ c-basic-offset: 2; indent-tabs-mode: nil; -*-
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

#include <drizzled/charset_info.h>
#include <drizzled/error.h>
#include <drizzled/function/cast/unsigned.h>
#include <drizzled/lex_string.h>

namespace drizzled {
namespace function {
namespace cast {

void Unsigned::print(String *str)
{
  str->append(STRING_WITH_LEN("cast("));
  args[0]->print(str);
  str->append(STRING_WITH_LEN(" as unsigned)"));

}


int64_t Unsigned::val_int()
{
  int64_t value= 0;
  int error= 0;
  char buff[MAX_FIELD_WIDTH];
  String tmp(buff,sizeof(buff), &my_charset_bin), *res; 

  /* 
    For a string result, we must first get the string and then convert it 
    to a longlong 
   */ 

  if (not (res= args[0]->val_str(&tmp))) 
  { 
    null_value= true; 
    error= 0; 
    return 0; 
  } 
  null_value= false; 

  char *end= res->ptr() + res->length(); 

  if (res->length() and ((char *)res->ptr())[0] == '-')
  {
    error= -1;
  }
  else
  {
    value= res->charset()->cset->strntoull10rnd(res->charset(),
                                                res->ptr(),
                                                res->length(),
                                                true, &end, &error);
  }

  if (error or (end != res->ptr() + res->length())) 
  {
    my_error(ER_INVALID_CAST_TO_UNSIGNED, MYF(0), res->c_str());
  }

  return value;
}

} // namespace cast
} // namespace function
} // namespace drizzled
