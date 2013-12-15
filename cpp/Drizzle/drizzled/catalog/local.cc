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

#include <drizzled/catalog/local.h>
#include <drizzled/plugin/catalog.h>

#include <boost/thread/once.hpp>

namespace drizzled
{
namespace catalog
{

/* Setup the local catalog for us to use with session */
static identifier::Catalog default_catalog("LOCAL");
static catalog::Instance::shared_ptr _local_catalog;

static boost::once_flag run_once= BOOST_ONCE_INIT;

static void init()
{
  _local_catalog= plugin::Catalog::getInstance(default_catalog);
}

identifier::Catalog::const_reference local_identifier()
{
  return default_catalog;
}

Instance::shared_ptr local()
{
  boost::call_once(&init, run_once);

  return _local_catalog;
}

} /* namespace catalog */
} /* namespace drizzled */
