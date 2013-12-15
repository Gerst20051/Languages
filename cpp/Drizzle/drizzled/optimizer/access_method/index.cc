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

#include <config.h>
#include <drizzled/session.h>
#include <drizzled/join_table.h>
#include <drizzled/sql_select.h>
#include <drizzled/optimizer/access_method/index.h>

using namespace drizzled;

bool optimizer::Index::getStats(Table *table,
                                JoinTable *join_tab)
{
  table->status= STATUS_NO_RECORD;
  if (join_tab->select)
  {
    delete join_tab->select->quick;
    join_tab->select->quick= 0;
  }

  delete join_tab->quick;
  join_tab->quick= 0;

  if (table->covering_keys.test(join_tab->ref.key) && 
      ! table->no_keyread)
  {
    table->key_read= 1;
    table->cursor->extra(HA_EXTRA_KEYREAD);
  }

  if (join_tab->type == AM_REF)
  {
    join_tab->read_first_record= join_read_always_key;
    join_tab->read_record.read_record= join_tab->insideout_match_tab ?
      join_read_next_same_diff : join_read_next_same;
  }
  else
  {
    join_tab->read_first_record= join_read_always_key_or_null;
    join_tab->read_record.read_record= join_read_next_same_or_null;
  }

  return false;
}
