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

#include <drizzled/plugin/plugin.h>
#include <drizzled/identifier.h>
#include <drizzled/catalog/instance.h>
#include <drizzled/catalog/engine.h>

#include <drizzled/visibility.h>

namespace drizzled
{
namespace plugin
{

/* 
  This object handles the aggregate of all operations for any/all of the Catalog engines.
*/
class DRIZZLED_API Catalog : public Plugin {
  /* Disable default constructors */
  Catalog();
  Catalog(const Catalog &);
  Catalog& operator=(const Catalog &);

public:
  typedef std::vector<Catalog *> vector;

  explicit Catalog(std::string name_arg) :
    Plugin(name_arg, "Catalog")
  {}
  virtual ~Catalog();

  virtual catalog::Engine::shared_ptr engine()= 0;

  static bool create(identifier::Catalog::const_reference);
  static bool create(identifier::Catalog::const_reference, message::catalog::shared_ptr &);
  static bool drop(identifier::Catalog::const_reference);

  static bool lock(identifier::Catalog::const_reference);
  static bool unlock(identifier::Catalog::const_reference);

  // Required for plugin interface
  static bool addPlugin(plugin::Catalog *plugin);
  static void removePlugin(plugin::Catalog *plugin);

  // Get Meta information
  static bool exist(identifier::Catalog::const_reference);
  static void getIdentifiers(identifier::Catalog::vector &identifiers);
  static void getMessages(message::catalog::vector &messages);
  static message::catalog::shared_ptr getMessage(identifier::Catalog::const_reference);

  // Get Instance
  static catalog::Instance::shared_ptr getInstance(identifier::Catalog::const_reference);
};

} /* namespace plugin */
} /* namespace drizzled */

