/* Copyright (C) 2000 MySQL AB

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA */

/*
  Defines: int64_t2str();

  int64_t2str(dst, radix, val)
  converts the (int64_t) integer "val" to character form and moves it to
  the destination string "dst" followed by a terminating NUL.  The
  result is normally a pointer to this NUL character, but if the radix
  is dud the result will be NULL and nothing will be changed.

  If radix is -2..-36, val is taken to be SIGNED.
  If radix is  2.. 36, val is taken to be UNSIGNED.
  That is, val is signed if and only if radix is.  You will normally
  use radix -10 only through itoa and ltoa, for radix 2, 8, or 16
  unsigned is what you generally want.

  _dig_vec is public just in case someone has a use for it.
  The definitions of itoa and ltoa are actually macros in m_string.h,
  but this is where the code is.

  Note: The standard itoa() returns a pointer to the argument, when int2str
	returns the pointer to the end-null.
	itoa assumes that 10 -base numbers are allways signed and other arn't.
*/

#include <config.h>

#include "m_string.h"

namespace drizzled
{
namespace internal
{

#if !defined(int64_t2str) && !defined(HAVE_LONGLONG2STR)

char _dig_vec_upper[] =
  "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

/*
  This assumes that int64_t multiplication is faster than int64_t division.
*/

char *int64_t2str(int64_t val,char *dst,int radix)
{
  char buffer[65];
  long long_val;
  uint64_t uval= (uint64_t) val;

  if (radix < 0)
  {
    if (radix < -36 || radix > -2) return (char*) 0;
    if (val < 0) {
      *dst++ = '-';
      /* Avoid integer overflow in (-val) for LONGLONG_MIN (BUG#31799). */
      uval = (uint64_t)0 - uval;
    }
    radix = -radix;
  }
  else
  {
    if (radix > 36 || radix < 2) return (char*) 0;
  }
  if (uval == 0)
  {
    *dst++='0';
    *dst='\0';
    return dst;
  }
  char* p = &buffer[sizeof(buffer)-1];
  *p = '\0';

  while (uval > (uint64_t) LONG_MAX)
  {
    uint64_t quo= uval/(uint32_t) radix;
    uint32_t rem= (uint32_t) (uval- quo* (uint32_t) radix);
    *--p = _dig_vec_upper[rem];
    uval= quo;
  }
  long_val= (long) uval;
  while (long_val != 0)
  {
    long quo= long_val/radix;
    *--p = _dig_vec_upper[(unsigned char) (long_val - quo*radix)];
    long_val= quo;
  }
  while ((*dst++ = *p++) != 0) ;
  return dst-1;
}

#endif

#ifndef int64_t10_to_str
char *int64_t10_to_str(int64_t val,char *dst,int radix)
{
  char buffer[65];
  long long_val;
  uint64_t uval= (uint64_t) val;

  if (radix < 0)
  {
    if (val < 0)
    {
      *dst++ = '-';
      /* Avoid integer overflow in (-val) for LONGLONG_MIN (BUG#31799). */
      uval = (uint64_t)0 - uval;
    }
  }

  if (uval == 0)
  {
    *dst++='0';
    *dst='\0';
    return dst;
  }
  char* p = &buffer[sizeof(buffer)-1];
  *p = '\0';

  while (uval > (uint64_t) LONG_MAX)
  {
    uint64_t quo= uval/(uint32_t) 10;
    uint32_t rem= (uint32_t) (uval- quo* (uint32_t) 10);
    *--p = _dig_vec_upper[rem];
    uval= quo;
  }
  long_val= (long) uval;
  while (long_val != 0)
  {
    long quo= long_val/10;
    *--p = _dig_vec_upper[(unsigned char) (long_val - quo*10)];
    long_val= quo;
  }
  while ((*dst++ = *p++) != 0) ;
  return dst-1;
}
#endif

} /* namespace internal */
} /* namespace drizzled */
