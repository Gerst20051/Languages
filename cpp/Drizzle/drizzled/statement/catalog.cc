/* - mode: c; c-basic-offset: 2; indent-tabs-mode: nil; -*-
 *  vim:expandtab:shiftwidth=2:tabstop=2:smarttab:
 *
 *  Copyright (C) 2011 Brian Aker
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
#include <drizzled/statement/catalog.h>

namespace drizzled
{

namespace statement
{

Catalog::Catalog(Session *in_session, drizzled::lex_string_t &arg) :
  Statement(in_session),
  _identifier(arg)
{
  set_command(SQLCOM_END);
}

bool Catalog::execute()
{
  if (not authorized())
    return false;

  if (not perform())
    return false;

  return true;
}

} /* namespace statement */
} /* namespace drizzled */
