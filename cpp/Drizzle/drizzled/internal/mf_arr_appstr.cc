/* Copyright (C) 2007 MySQL AB

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

#include <config.h>

#include <drizzled/internal/my_sys.h>
#include <drizzled/internal/m_string.h>

namespace drizzled
{
namespace internal
{

/**
  Append str to array, or move to the end if it already exists

  @param str    String to be appended
  @param array  The array, terminated by a NULL element, all unused elements
                pre-initialized to NULL
  @param size   Size of the array; array must be terminated by a NULL
                pointer, so can hold size - 1 elements

  @retval false  Success
  @retval true   Failure, array is full
*/

bool array_append_string_unique(const char *str,
                                   const char **array, size_t size)
{
  const char **p;
  /* end points at the terminating NULL element */
  const char **end= array + size - 1;
  assert(*end == NULL);

  for (p= array; *p; ++p)
  {
    if (strcmp(*p, str) == 0)
      break;
  }
  if (p >= end)
    return true;                               /* Array is full */

  assert(*p == NULL || strcmp(*p, str) == 0);

  while (*(p + 1))
  {
    *p= *(p + 1);
    ++p;
  }

  assert(p < end);
  *p= str;

  return false;                                 /* Success */
}

} /* namespace internal */
} /* namespace drizzled */
