/* - mode: c; c-basic-offset: 2; indent-tabs-mode: nil; -*-
 *  vim:expandtab:shiftwidth=2:tabstop=2:smarttab:
 *
 *  Copyright (C) 2009 Sun Microsystems, Inc.
 *
 *  Authors:
 *
 *  Jay Pipes <joinfu@sun.com>
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

/**
 * @file
 *
 * Implements the PRINT_TRANSACTION_MESSAGE(filename, offset) UDF.
 */

#include <config.h>
#include <drizzled/plugin/function.h>
#include <drizzled/item/func.h>
#include <drizzled/function/str/strfunc.h>
#include <drizzled/error.h>
#include <drizzled/internal/my_sys.h>
#include <drizzled/charset.h>
#include <drizzled/gettext.h>
#include <drizzled/errmsg_print.h>

#include <fcntl.h>
#include <errno.h>

#include "transaction_log.h"
#include "print_transaction_message.h"

#include <drizzled/message/transaction.pb.h>
#include <drizzled/replication_services.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/text_format.h>

using namespace std;
using namespace drizzled;
using namespace google;

/** Defined in transaction_log.cc */
extern TransactionLog *transaction_log;

plugin::Create_function<PrintTransactionMessageFunction> *print_transaction_message_func_factory= NULL;

void PrintTransactionMessageFunction::fix_length_and_dec()
{
  max_length= 2 * 1024 * 1024; /* 2MB size limit seems ok... */
  args[0]->collation.set(
    get_charset_by_csname(args[0]->collation.collation->csname,
                          MY_CS_BINSORT), DERIVATION_COERCIBLE);
}

String *PrintTransactionMessageFunction::val_str(String *str)
{
  assert(fixed == true);

  String *filename_arg= args[0]->val_str(str);
  off_t offset_arg= static_cast<int64_t>(args[1]->val_int());

  if (filename_arg == NULL || args[1]->null_value == true)
  {
    my_error(ER_INVALID_NULL_ARGUMENT, MYF(0), func_name());
    null_value= true;
    return NULL;
  }

  if (transaction_log == NULL)
  {
    my_error(ER_INVALID_NULL_ARGUMENT, MYF(0), func_name());
    null_value= true;
    return NULL;
  }

  null_value= false;

  message::Transaction transaction_message;

  /**
   * @todo Of course, this is not efficient to create a
   * new input stream every time we call the UDF.  Create
   * a pool of TransactionLogReader objects that can be 
   * re-used.
   */
  const string &filename= transaction_log->getLogFilename();
  int log_file= open(filename.c_str(), O_RDONLY);
  if (log_file == -1)
  {
    sql_perror(_("Failed to open transaction log file"), filename);
    null_value= true;

    return NULL;
  }

  (void) lseek(log_file, offset_arg, SEEK_SET);

  protobuf::io::FileInputStream *file_input= new protobuf::io::FileInputStream(log_file);
  file_input->SetCloseOnDelete(true);

  protobuf::io::CodedInputStream *coded_input= new protobuf::io::CodedInputStream(file_input);

  /* Grab our message type and length */
  uint32_t message_type;
  if (! coded_input->ReadLittleEndian32(&message_type))
  {
    delete coded_input;
    delete file_input;

    /** @todo Error message for this... */
    null_value= true;
    return NULL;
  }

  /* Validate message type */
  if (message_type != ReplicationServices::TRANSACTION)
  {
    fprintf(stderr, _("GPB message is not a valid type.\n"));
    delete coded_input;
    delete file_input;
    null_value= true;
    return NULL;
  }

  uint32_t message_size;
  if (! coded_input->ReadLittleEndian32(&message_size))
  {
    delete coded_input;
    delete file_input;

    /** @todo Error message for this... */
    null_value= true;
    return NULL;
  }

  if (message_size > INT_MAX)
  {
    fprintf(stderr, _("GPB message is not a valid size.\n"));
    delete coded_input;
    delete file_input;
    null_value= true;
    return NULL;
  }

  uint8_t *buffer= (uint8_t *) malloc(message_size);

  bool result= coded_input->ReadRaw(buffer, message_size);
  if (result == false)
  {
    // 120 was arbitrary
    sql_perror(_("Could not read transaction message. Raw buffer read "), std::string((const char *)buffer, std::min(message_size, 120U)));
  }

  result= transaction_message.ParseFromArray(buffer, static_cast<int32_t>(message_size));
  if (result == false)
  {
    fprintf(stderr, _("Unable to parse transaction. Got error: %s.\n"), transaction_message.InitializationErrorString().c_str());
    if (buffer != NULL)
      fprintf(stderr, _("BUFFER: %s\n"), buffer);
  }

  free(buffer);

  string transaction_text;
  protobuf::TextFormat::PrintToString(transaction_message, &transaction_text);

  if (str->alloc(transaction_text.length()))
  {
    delete coded_input;
    delete file_input;
    null_value= true;
    return NULL;
  }

  str->length(transaction_text.length());

  strncpy(str->ptr(), transaction_text.c_str(), transaction_text.length());

  delete coded_input;
  delete file_input;

  return str;
}
