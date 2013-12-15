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

#include <plugin/schema_engine/schema.h>

#include <string>

using namespace std;
using namespace drizzled;

static drizzled::plugin::StorageEngine *schema_plugin= NULL;

static int init(drizzled::module::Context &context)
{
  schema_plugin= new(std::nothrow) Schema();

  if (not schema_plugin)
  {
    return 1;
  }

  context.add(schema_plugin);

  return 0;
}

DRIZZLE_DECLARE_PLUGIN
{
  DRIZZLE_VERSION_ID,
  "SchemaEngine",
  "1.0",
  "Brian Aker",
  "This implements the default file based Schema engine.",
  PLUGIN_LICENSE_GPL,
  init,     /* Plugin Init */
  "signal_handler",               /* depends */
  NULL                /* config options   */
}
DRIZZLE_DECLARE_PLUGIN_END;
