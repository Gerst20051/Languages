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
  Radixsort for pointers to fixed length strings.
  A very quick sort for not to long (< 20 char) strings.
  Neads a extra buffers of number_of_elements pointers but is
  2-3 times faster than quicksort
*/

#include <config.h>

#include <drizzled/internal/my_sys.h>
#include <drizzled/internal/m_string.h>

namespace drizzled
{
namespace internal
{

	/* Radixsort */

void radixsort_for_str_ptr(unsigned char **base, uint32_t number_of_elements, size_t size_of_element, unsigned char **buffer)
{
  unsigned char **end,**ptr,**buffer_ptr;
  uint32_t *count_ptr,*count_end,count[256];
  int pass;

  end=base+number_of_elements; count_end=count+256;
  for (pass=(int) size_of_element-1 ; pass >= 0 ; pass--)
  {
    memset(count, 0, sizeof(uint32_t)*256);
    for (ptr= base ; ptr < end ; ptr++)
      count[ptr[0][pass]]++;
    if (count[0] == number_of_elements)
      goto next;
    for (count_ptr=count+1 ; count_ptr < count_end ; count_ptr++)
    {
      if (*count_ptr == number_of_elements)
	goto next;
      (*count_ptr)+= *(count_ptr-1);
    }
    for (ptr= end ; ptr-- != base ;)
      buffer[--count[ptr[0][pass]]]= *ptr;
    for (ptr=base, buffer_ptr=buffer ; ptr < end ;)
      (*ptr++) = *buffer_ptr++;
  next:;
  }
}

} /* namespace internal */
} /* namespace drizzled */
