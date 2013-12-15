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

#include <drizzled/session.h>
#include <plugin/string_functions/functions.h>

namespace drizzled
{
namespace string_functions
{

bool Regex::val_bool()
{
  drizzled::String *res= args[0]->val_str(&_res);

  if (re.empty())
  {
    drizzled::String _regex;
    drizzled::String *regex= args[1]->val_str(&_regex);

    if (not regex or not regex->length() or not res or not res->length())
    {
      null_value= true;
      return false;
    }

    re.assign(regex->c_str());
  }

  if (not res or not res->length())
  {
    null_value= true;
    return false;
  }

  null_value= false;

  if (is_negative)
    return not boost::regex_match(res->c_str(), re);

  return boost::regex_match(res->c_str(), re);
}

} // namespace string_functions
} // namespace drizzled
