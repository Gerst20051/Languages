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

/**
 * @file
 * @brief Memory root declarations
 */



#pragma once

#include <cstddef>

#include <drizzled/definitions.h>

#include <drizzled/visibility.h>

namespace drizzled
{

/**
 * @namespace drizzled::memory
 * Memory allocation utils
 *
 * NB: This namespace documentation may not seem very useful, but without a
 * comment on the namespace Doxygen won't extract any documentation for
 * namespace members.
 */
namespace memory
{

static const int KEEP_PREALLOC= 1;
/* move used to free list and reuse them */
static const int MARK_BLOCKS_FREE= 2;

namespace internal
{

class UsedMemory
{			   /* struct for once_alloc (block) */
public:
  UsedMemory *next;	   /* Next block in use */
  size_t left;		   /* memory left in block  */            
  size_t size;		   /* size of block */
};

}

static const size_t ROOT_MIN_BLOCK_SIZE= (MALLOC_OVERHEAD + sizeof(internal::UsedMemory) + 8);



class DRIZZLED_API Root
{
public:

  Root() :
    free(0),
    used(0),
    pre_alloc(0),
    min_malloc(0),
    block_size(0),
    block_num(0),
    first_block_usage(0),
    error_handler(0)
  { }

  Root(size_t block_size_arg)
  {
    free= used= pre_alloc= 0;
    min_malloc= 32;
    block_size= block_size_arg - memory::ROOT_MIN_BLOCK_SIZE;
    block_num= 4;			/* We shift this with >>2 */
    first_block_usage= 0;
    error_handler= 0;
  }

  ~Root();

  /**
   * blocks with free memory in it 
   */
  internal::UsedMemory *free;

  /**
   * blocks almost without free memory 
   */
  internal::UsedMemory *used;

  /**
   * preallocated block 
   */
  internal::UsedMemory *pre_alloc;

  /**
   * if block have less memory it will be put in 'used' list 
   */
  size_t min_malloc;

  size_t block_size;         ///< initial block size
  unsigned int block_num;    ///< allocated blocks counter 

  /**
   * first free block in queue test counter (if it exceed
   * MAX_BLOCK_USAGE_BEFORE_DROP block will be dropped in 'used' list)
   */
  unsigned int first_block_usage;

  void (*error_handler)(void);
  void reset_root_defaults(size_t block_size, size_t prealloc_size);
  void *alloc_root(size_t Size);
  inline void *allocate(size_t Size)
  {
    return alloc_root(Size);
  }
  void mark_blocks_free();
  void *memdup_root(const void *str, size_t len);
  char *strdup_root(const char *str);

  char *strmake_root(const char *str,size_t len);
  void init_alloc_root(size_t block_size= ROOT_MIN_BLOCK_SIZE);

  inline void *duplicate(const void *str, size_t len)
  {
    return memdup_root(str, len);
  }

  inline bool alloc_root_inited()
  {
    return min_malloc != 0;
  }
  void free_root(myf MyFLAGS);
  void *multi_alloc_root(int unused, ...);
};

} /* namespace memory */
} /* namespace drizzled */

