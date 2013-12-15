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

#include <drizzled/definitions.h>
#include <drizzled/sql_list.h>
#include <drizzled/item.h>

namespace drizzled
{

class TableList;
class file_exchange;
class Session;
class Table;
class CopyInfo;

int load(Session *session, file_exchange *ex, TableList *table_list,
               List<Item> &fields_vars, List<Item> &set_fields,
               List<Item> &set_values_list,
               enum enum_duplicates handle_duplicates, bool ignore);
    
int write_record(Session *session, Table *table, CopyInfo *info);

} /* namespace drizzled */

