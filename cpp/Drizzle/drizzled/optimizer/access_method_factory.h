/* -*- mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; -*-
 *  vim:expandtab:shiftwidth=2:tabstop=2:smarttab:
 *
 *  Copyright (C) 2010 Padraig O'Sullivan
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

#pragma once

#include <drizzled/definitions.h>
#include <drizzled/error.h>
#include <drizzled/sql_parse.h>
#include <drizzled/sql_base.h>
#include <drizzled/join_table.h>
#include <drizzled/optimizer/access_method.h>

#include <boost/shared_ptr.hpp>

/* Forward declarations */
class Table;
class JoinTable;

namespace drizzled
{
namespace optimizer
{

/**
 * @class AccessMethodFactory
 * @brief Creates access method objects
 */
class AccessMethodFactory
{
public:

  static AccessMethodFactory &singleton()
  {
    static AccessMethodFactory fact;
    return fact;
  }

  boost::shared_ptr<AccessMethod> createAccessMethod(enum access_method type);

private:
  
  AccessMethodFactory() {}

  ~AccessMethodFactory() {}

  AccessMethodFactory(const AccessMethodFactory&);
};

} /* end namespace optimizer */

} /* end namespace drizzled */

