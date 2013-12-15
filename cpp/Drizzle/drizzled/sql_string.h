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

/* This file is originally from the mysql distribution. Coded by monty */

#include <drizzled/common.h>

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <string>

#include <drizzled/visibility.h>

#ifndef NOT_FIXED_DEC
#define NOT_FIXED_DEC			(uint8_t)31
#endif

namespace drizzled
{

class String;

extern DRIZZLED_API String my_empty_string;
extern const String my_null_string;
namespace memory { class Root; }
typedef struct charset_info_st CHARSET_INFO;

DRIZZLED_API std::string String_to_std_string(String const& s);
DRIZZLED_API String* set_String_from_std_string(String* s, std::string const& cs);

int sortcmp(const String *a,const String *b, const CHARSET_INFO * const cs);
int stringcmp(const String *a,const String *b);
String *copy_if_not_alloced(String *a,String *b,size_t arg_length);
size_t well_formed_copy_nchars(const CHARSET_INFO * const to_cs,
                                 char *to, size_t to_length,
                                 const CHARSET_INFO * const from_cs,
                                 const char *from, size_t from_length,
                                 size_t nchars,
                                 const char **well_formed_error_pos,
                                 const char **cannot_convert_error_pos,
                                 const char **from_end_pos);


class DRIZZLED_API String
{
  char *Ptr;
  size_t str_length,Alloced_length;
  bool alloced;
  const CHARSET_INFO *str_charset;

public:
  String();
  String(size_t length_arg);
  String(const char *str, const CHARSET_INFO * const cs);
  String(const char *str, size_t len, const CHARSET_INFO * const cs);
  String(char *str, size_t len, const CHARSET_INFO * const cs);
  String(const String &str);

  static void *operator new(size_t size, memory::Root *mem_root);
  static void operator delete(void *, size_t)
  { }
  static void operator delete(void *, memory::Root *)
  { }
  ~String();

  inline void set_charset(const CHARSET_INFO * const charset_arg)
  { str_charset= charset_arg; }
  inline const CHARSET_INFO *charset() const { return str_charset; }
  inline size_t length() const { return str_length;}
  inline size_t alloced_length() const { return Alloced_length;}
  inline char& operator [] (size_t i) const { return Ptr[i]; }
  inline void length(size_t len) { str_length=len ; }
  inline bool is_empty() { return (str_length == 0); }
  inline void mark_as_const() { Alloced_length= 0;}
  inline char *ptr() { return Ptr; }
  inline const char *ptr() const { return Ptr; }
  inline char *c_ptr()
  {
    if (str_length == Alloced_length)
      (void) realloc(str_length);
    else
      Ptr[str_length]= 0;

    return Ptr;
  }
  inline char *c_ptr_quick()
  {
    if (Ptr && str_length < Alloced_length)
      Ptr[str_length]=0;
    return Ptr;
  }
  inline char *c_ptr_safe()
  {
    if (Ptr && str_length < Alloced_length)
      Ptr[str_length]=0;
    else
      (void) realloc(str_length);
    return Ptr;
  }
  inline char *c_str()
  {
    if (Ptr && str_length < Alloced_length)
      Ptr[str_length]=0;
    else
      (void) realloc(str_length);
    return Ptr;
  }
  void append_identifier(const char *name, size_t length);

  void set(String &str,size_t offset,size_t arg_length)
  {
    assert(&str != this);
    free();
    Ptr= str.ptr()+offset; str_length=arg_length; alloced=0;
    if (str.Alloced_length)
      Alloced_length=str.Alloced_length-offset;
    else
      Alloced_length=0;
    str_charset=str.str_charset;
  }
  inline void set(char *str,size_t arg_length, const CHARSET_INFO * const cs)
  {
    free();
    Ptr= str; str_length=Alloced_length=arg_length ; alloced=0;
    str_charset=cs;
  }
  inline void set(const char *str,size_t arg_length, const CHARSET_INFO * const cs)
  {
    free();
    Ptr= const_cast<char*>(str);
    str_length=arg_length; Alloced_length=0 ; alloced=0;
    str_charset=cs;
  }
  bool set_ascii(const char *str, size_t arg_length);
  inline void set_quick(char *str,size_t arg_length, const CHARSET_INFO * const cs)
  {
    if (!alloced)
    {
      Ptr= str; str_length= Alloced_length= arg_length;
    }
    str_charset= cs;
  }
  bool set_int(int64_t num, bool unsigned_flag, const CHARSET_INFO * const cs);
  bool set(int64_t num, const CHARSET_INFO * const cs)
  { return set_int(num, false, cs); }
  bool set(uint64_t num, const CHARSET_INFO * const cs)
  { return set_int(static_cast<int64_t>(num), true, cs); }
  bool set_real(double num,size_t decimals, const CHARSET_INFO * const cs);

  /*
    PMG 2004.11.12
    This is a method that works the same as perl's "chop". It simply
    drops the last character of a string. This is useful in the case
    of the federated storage handler where I'm building a unknown
    number, list of values and fields to be used in a sql insert
    statement to be run on the remote server, and have a comma after each.
    When the list is complete, I "chop" off the trailing comma

    ex.
      String stringobj;
      stringobj.append("VALUES ('foo', 'fi', 'fo',");
      stringobj.chop();
      stringobj.append(")");

    In this case, the value of string was:

    VALUES ('foo', 'fi', 'fo',
    VALUES ('foo', 'fi', 'fo'
    VALUES ('foo', 'fi', 'fo')

  */
  inline void chop()
  {
    Ptr[str_length--]= '\0';
  }

  inline void free()
  {
    if (alloced)
    {
      alloced=0;
      Alloced_length=0;
      ::free(Ptr);
      Ptr=0;
      str_length=0;				/* Safety */
    }
  }
  inline bool alloc(size_t arg_length)
  {
    if (arg_length < Alloced_length)
      return 0;
    return real_alloc(arg_length);
  }
  bool real_alloc(size_t arg_length);			// Empties old string
  bool realloc(size_t arg_length);
  inline void shrink(size_t arg_length)		// Shrink buffer
  {
    if (arg_length < Alloced_length)
    {
      char *new_ptr;
      if (!(new_ptr= reinterpret_cast<char*>(::realloc(Ptr,arg_length))))
      {
        Alloced_length = 0;
        real_alloc(arg_length);
      }
      else
      {
        Ptr=new_ptr;
        Alloced_length=arg_length;
      }
    }
  }
  bool is_alloced() { return alloced; }
  inline String& operator = (const String &s)
  {
    if (&s != this)
    {
      /*
        It is forbidden to do assignments like
        some_string = substring_of_that_string
       */
      assert(!s.uses_buffer_owned_by(this));
      free();
      Ptr=s.Ptr ; str_length=s.str_length ; Alloced_length=s.Alloced_length;
      alloced=0;
    }
    return *this;
  }

  bool copy();					// Alloc string if not alloced
  bool copy(const String &s);			// Allocate new string
  bool copy(const std::string&, const CHARSET_INFO * const cs);	// Allocate new string
  bool copy(const char *s,size_t arg_length, const CHARSET_INFO * const cs);	// Allocate new string
  static bool needs_conversion(size_t arg_length,
  			       const CHARSET_INFO * const cs_from, const CHARSET_INFO * const cs_to,
			       size_t *offset);
  bool set_or_copy_aligned(const char *s, size_t arg_length, const CHARSET_INFO * const cs);
  bool copy(const char*s,size_t arg_length, const CHARSET_INFO * const csfrom,
            const CHARSET_INFO * const csto, size_t *errors);
  bool append(const String &s);
  bool append(const char *s);
  bool append(const char *s,size_t arg_length);
  bool append(const char *s,size_t arg_length, const CHARSET_INFO * const cs);
  bool append_with_prefill(const char *s, size_t arg_length,
			   size_t full_length, char fill_char);
  int strstr(const String &search,size_t offset=0); // Returns offset to substring or -1
  int strrstr(const String &search,size_t offset=0); // Returns offset to substring or -1
  bool replace(size_t offset,size_t arg_length,const char *to,size_t length);
  bool replace(size_t offset,size_t arg_length,const String &to);
  inline bool append(char chr)
  {
    if (str_length < Alloced_length)
    {
      Ptr[str_length++]=chr;
    }
    else
    {
      if (realloc(str_length+1))
        return 1;
      Ptr[str_length++]=chr;
    }
    return 0;
  }
  friend int sortcmp(const String *a,const String *b, const CHARSET_INFO * const cs);
  friend int stringcmp(const String *a,const String *b);
  friend String *copy_if_not_alloced(String *a,String *b,size_t arg_length);
  size_t numchars();
  int charpos(int i,size_t offset=0);

  int reserve(size_t space_needed)
  {
    return realloc(str_length + space_needed);
  }
  int reserve(size_t space_needed, size_t grow_by);

  /*
    The following append operations do NOT check alloced memory
    q_*** methods writes values of parameters itself
    qs_*** methods writes string representation of value
  */
  void q_append(const char c);
  void q_append(const size_t n);
  void q_append(double d);
  void q_append(double *d);
  void q_append(const char *data, size_t data_len);
  void write_at_position(int position, size_t value);

  /* Inline (general) functions used by the protocol functions */

  inline char *prep_append(size_t arg_length, size_t step_alloc)
  {
    size_t new_length= arg_length + str_length;
    if (new_length > Alloced_length)
    {
      if (realloc(new_length + step_alloc))
        return 0;
    }
    size_t old_length= str_length;
    str_length+= arg_length;
    return Ptr+ old_length;			/* Area to use */
  }

  inline bool append(const char *s, size_t arg_length, size_t step_alloc)
  {
    size_t new_length= arg_length + str_length;
    if (new_length > Alloced_length && realloc(new_length + step_alloc))
      return true;
    memcpy(Ptr+str_length, s, arg_length);
    str_length+= arg_length;
    return false;
  }
  void print(String *print);

  /* Swap two string objects. Efficient way to exchange data without memcpy. */
  void swap(String &s);

  inline bool uses_buffer_owned_by(const String *s) const
  {
    return (s->alloced && Ptr >= s->Ptr && Ptr < s->Ptr + s->str_length);
  }
};

bool check_if_only_end_space(const CHARSET_INFO * const cs, char *str,
                             char *end);

std::ostream& operator<<(std::ostream& output, const String &str);

} /* namespace drizzled */

bool operator==(const drizzled::String &s1, const drizzled::String &s2);
bool operator!=(const drizzled::String &s1, const drizzled::String &s2);


