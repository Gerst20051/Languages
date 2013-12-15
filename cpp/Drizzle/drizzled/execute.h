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

#include <drizzled/visibility.h>

namespace drizzled
{

namespace sql
{
  class ResultSet;
}

class DRIZZLED_API Execute
{
  bool wait;
  Session &_session;

public:
  Execute(Session&, bool wait_arg);
  ~Execute();

  void run(std::string &to_execute);
  void run(const char *arg, size_t length);
  void run(std::string &execution_string, sql::ResultSet &result_set);

  Session &session()
  {
    return _session;
  }

  void setWait(bool arg= true)
  {
    wait= arg;
  }

private:
};

} /* namespace drizzled */

