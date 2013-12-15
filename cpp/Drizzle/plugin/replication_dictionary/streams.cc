/* - mode: c; c-basic-offset: 2; indent-tabs-mode: nil; -*-
 *  vim:expandtab:shiftwidth=2:tabstop=2:smarttab:
 *
 * Copyright (C) 2010 Jay Pipes <jaypipes@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *   * Neither the name of Drizzle nor the names of its contributors
 *     may be used to endorse or promote products derived from this software
 *     without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <config.h>
#include "streams.h"

#include <drizzled/plugin/transaction_replicator.h>
#include <drizzled/plugin/transaction_applier.h>

using namespace std;
using namespace drizzled;

ReplicationStreamsTool::ReplicationStreamsTool() :
  plugin::TableFunction("DATA_DICTIONARY", "REPLICATION_STREAMS")
{
  add_field("REPLICATOR");
  add_field("APPLIER");
}

ReplicationStreamsTool::Generator::Generator(Field **arg) :
  plugin::TableFunction::Generator(arg)
{
  ReplicationServices &replication_services= ReplicationServices::singleton();
  ReplicationServices::ReplicationStreams &replication_streams=
    replication_services.getReplicationStreams();

  it= replication_streams.begin();
  end= replication_streams.end();
}

bool ReplicationStreamsTool::Generator::populate()
{
  if (it == end)
    return false;

  plugin::TransactionReplicator *replicator= (*it).first;
  plugin::TransactionApplier *applier= (*it).second;

  /* REPLICATOR */
  push(replicator->getName());

  /* APPLIER */
  push(applier->getName());

  ++it;

  return true;
}