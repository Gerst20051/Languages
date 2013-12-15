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

#include <drizzled/message/schema.h>

namespace drizzled {

class Session;
namespace message { class Schema; }
namespace identifier { class Schema; }

namespace schema {

bool create(Session &session, const message::Schema &schema_message, const bool is_if_not_exists);
bool alter(Session &session, const message::Schema &schema_message, const message::Schema &original_schema);
bool drop(Session &session, identifier::Schema &identifier, const bool if_exists);
bool change(Session &session, identifier::Schema &identifier);
bool check(Session &session, identifier::Schema &schema);

}

} /* namespace drizzled */

