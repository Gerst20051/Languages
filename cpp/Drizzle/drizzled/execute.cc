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

#include <drizzled/session.h>
#include <drizzled/user_var_entry.h>
#include <drizzled/plugin/client/cached.h>
#include <drizzled/plugin/client/concurrent.h>
#include <drizzled/catalog/local.h>
#include <drizzled/execute.h>

namespace drizzled
{

Execute::Execute(Session &arg, bool wait_arg) :
  wait(wait_arg),
  _session(arg)
{
}

Execute::~Execute()
{
}

void Execute::run(const char *arg, size_t length)
{
  std::string execution_string(arg, length);
  run(execution_string);
}

void Execute::run(std::string &execution_string, sql::ResultSet &result_set)
{
  boost_thread_shared_ptr thread;
  
  if (_session.isConcurrentExecuteAllowed())
  {
    plugin::client::Cached *client= new plugin::client::Cached(result_set);
    client->pushSQL(execution_string);
    Session::shared_ptr new_session= Session::make_shared(client, catalog::local());
    
    // We set the current schema.  @todo do the same with catalog
    util::string::const_shared_ptr schema(_session.schema());
    if (not schema->empty())
      new_session->set_db(*schema);
    
    new_session->setConcurrentExecute(false);
    
    // Overwrite the context in the next session, with what we have in our
    // session. Eventually we will allow someone to change the effective
    // user.
    new_session->user()= _session.user();
    
    if (Session::schedule(new_session))
    {
      Session::unlink(new_session);
    }
    else if (wait)
    {
      thread= new_session->getThread();
    }
  }
  else
  {
    my_error(ER_WRONG_ARGUMENTS, MYF(0), "A Concurrent Execution Session can not launch another session.");
    return;
  }
  
  if (wait && thread && thread->joinable())
  {
    // We want to make sure that we can be killed
    if (_session.getThread())
    {
      boost::this_thread::restore_interruption dl(_session.getThreadInterupt());
      
      try {
        thread->join();
      }
      catch(boost::thread_interrupted const&)
      {
        // Just surpress and return the error
        my_error(drizzled::ER_QUERY_INTERRUPTED, MYF(0));
        return;
      }
    }
    else
    {
      thread->join();
    }
  }
}

void Execute::run(std::string &execution_string)
{
  boost_thread_shared_ptr thread;

  if (_session.isConcurrentExecuteAllowed())
  {
    plugin::client::Concurrent *client= new plugin::client::Concurrent;
    client->pushSQL(execution_string);
    Session::shared_ptr new_session= Session::make_shared(client, catalog::local());

    // We set the current schema.  @todo do the same with catalog
    util::string::const_shared_ptr schema(_session.schema());
    if (not schema->empty())
      new_session->set_db(*schema);

    new_session->setConcurrentExecute(false);

    // Overwrite the context in the next session, with what we have in our
    // session. Eventually we will allow someone to change the effective
    // user.
    new_session->user()= _session.user();

    if (Session::schedule(new_session))
    {
      Session::unlink(new_session);
    }
    else if (wait)
    {
      thread= new_session->getThread();
    }
  }
  else
  {
    my_error(ER_WRONG_ARGUMENTS, MYF(0), "A Concurrent Execution Session can not launch another session.");
    return;
  }

  if (wait && thread && thread->joinable())
  {
    // We want to make sure that we can be killed
    if (_session.getThread())
    {
      boost::this_thread::restore_interruption dl(_session.getThreadInterupt());

      try {
        thread->join();
      }
      catch(boost::thread_interrupted const&)
      {
        // Just surpress and return the error
        my_error(drizzled::ER_QUERY_INTERRUPTED, MYF(0));
        return;
      }
    }
    else
    {
      thread->join();
    }
  }
}

} /* namespace drizzled */
