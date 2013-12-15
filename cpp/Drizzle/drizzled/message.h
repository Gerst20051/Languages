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

#pragma once

#include <drizzled/message/table.h>
#include <drizzled/message/schema.h>
#if 0
#include <drizzled/message/transaction.pb.h>
#include <google/protobuf/text_format.h>
#endif
#include <drizzled/identifier.h>

#include <boost/shared_ptr.hpp>

namespace drizzled {
namespace message {

void update(drizzled::message::Schema &arg);
void update(drizzled::message::Table &arg);

bool is_numeric(const drizzled::message::Table::Field &type);

const std::string &type(const drizzled::message::Table::Field &type);
const std::string &type(drizzled::message::Table::Field::FieldType type);
const std::string &type(drizzled::message::Table::ForeignKeyConstraint::ForeignKeyOption type);
const std::string &type(bool type);
const std::string &type(drizzled::message::Table::Index::IndexType type);
const std::string &type(drizzled::message::Table::ForeignKeyConstraint::ForeignKeyMatchOption type);
const std::string &type(drizzled::message::Table::TableType type);

template<class T> bool is_replicated(const T& reference)
{
  if (reference.has_replication_options() and
      reference.replication_options().has_is_replicated())
  {
    return reference.replication_options().is_replicated();
  }

  return true;
}

template<class T> void set_is_replicated(T& reference, bool arg)
{
  message::ReplicationOptions *options= reference.mutable_replication_options();
  options->set_is_replicated(arg);
}

#if 0
std::ostream& operator<<(std::ostream& output, const message::Transaction &message);
std::ostream& operator<<(std::ostream& output, const message::Table &message);
#endif


} /* namespace message */
} /* namespace drizzled */

