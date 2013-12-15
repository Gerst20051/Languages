/* Copyright (C) 2000-2006 MySQL AB

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

#include "myisam_priv.h"

using namespace drizzled;

	/*
	   Read next row with the same key as previous read, but abort if
	   the key changes.
	   One may have done a write, update or delete of the previous row.
	   NOTE! Even if one changes the previous row, the next read is done
	   based on the position of the last used key!
	*/

int mi_rnext_same(MI_INFO *info, unsigned char *buf)
{
  int error;
  uint32_t inx,not_used[2];
  MI_KEYDEF *keyinfo;

  if ((int) (inx=info->lastinx) < 0 || info->lastpos == HA_OFFSET_ERROR)
    return(errno=HA_ERR_WRONG_INDEX);
  keyinfo=info->s->keyinfo+inx;
  if (fast_mi_readinfo(info))
    return(errno);

  switch (keyinfo->key_alg)
  {
    case HA_KEY_ALG_BTREE:
    default:
      if (!(info->update & HA_STATE_RNEXT_SAME))
      {
        /* First rnext_same; Store old key */
        memcpy(info->lastkey2,info->lastkey,info->last_rkey_length);
      }
      for (;;)
      {
        if ((error=_mi_search_next(info,keyinfo,info->lastkey,
			       info->lastkey_length,SEARCH_BIGGER,
			       info->s->state.key_root[inx])))
          break;
        if (ha_key_cmp(keyinfo->seg, info->lastkey, info->lastkey2,
                       info->last_rkey_length, SEARCH_FIND, not_used))
        {
          error=1;
          errno=HA_ERR_END_OF_FILE;
          info->lastpos= HA_OFFSET_ERROR;
          break;
        }
        /* Skip rows that are inserted by other threads since we got a lock */
        if (info->lastpos < info->state->data_file_length &&
            (!info->index_cond_func || mi_check_index_cond(info, inx, buf)))
          break;
      }
  }
  /* Don't clear if database-changed */
  info->update&= (HA_STATE_CHANGED | HA_STATE_ROW_CHANGED);
  info->update|= HA_STATE_NEXT_FOUND | HA_STATE_RNEXT_SAME;

  if (error)
  {
    if (errno == HA_ERR_KEY_NOT_FOUND)
      errno=HA_ERR_END_OF_FILE;
  }
  else if (!buf)
  {
    return(info->lastpos==HA_OFFSET_ERROR ? errno : 0);
  }
  else if (!(*info->read_record)(info,info->lastpos,buf))
  {
    info->update|= HA_STATE_AKTIV;		/* Record is read */
    return(0);
  }
  return(errno);
} /* mi_rnext_same */
