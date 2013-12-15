/* -*- mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; -*-
 *  vim:expandtab:shiftwidth=2:tabstop=2:smarttab:
 *
 *  Copyright (C) 2009 Sun Microsystems, Inc.
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

#include <drizzled/plugin/plugin.h>
#include <drizzled/plugin/transaction_replicator.h>
#include <drizzled/replication_services.h>

namespace drizzled
{

bool plugin::TransactionReplicator::addPlugin(plugin::TransactionReplicator *replicator)
{
  ReplicationServices &replication_services= ReplicationServices::singleton();
  replication_services.attachReplicator(replicator);
  return false;
}

void plugin::TransactionReplicator::removePlugin(plugin::TransactionReplicator *replicator)
{
  ReplicationServices &replication_services= ReplicationServices::singleton();
  replication_services.detachReplicator(replicator);
}

} /* namespace drizzled */
