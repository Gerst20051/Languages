#include ".\minifmod.h"

#define KEYSTATE(n) ((GetAsyncKeyState(n) & 0x8000) ? TRUE: FALSE)

typedef struct
{
  int length;
  int pos;
  void *data;
}
MEMFILE;

unsigned int memopen(char *name)
{
  MEMFILE *memfile;

    HRSRC rec;
    HGLOBAL handle;
  memfile = (MEMFILE *)calloc(sizeof(MEMFILE),1);
    rec = FindResource(NULL, name, RT_RCDATA);
    handle = LoadResource(NULL, rec);

    memfile->data = LockResource(handle);
    memfile->length = SizeofResource(NULL, rec);
    memfile->pos = 0;
  return((unsigned int)memfile);
}

void memclose(unsigned int handle)
{
  MEMFILE *memfile = (MEMFILE *)handle;
  free(memfile->data); // dont free it if it was initialized with LockResource
}

int memread(void *buffer, int size, unsigned int handle)
{
  MEMFILE *memfile = (MEMFILE *)handle;

  if (memfile->pos + size >= memfile->length)
    size = memfile->length - memfile->pos;

  memcpy(buffer, (char *)memfile->data+memfile->pos, size);
  memfile->pos += size;

  return(size);
}

void memseek(unsigned int handle, int pos, signed char mode)
{
  MEMFILE *memfile = (MEMFILE *)handle;

  if (mode == SEEK_SET)
    memfile->pos = pos;
  else if (mode == SEEK_CUR)
    memfile->pos += pos;
  else if (mode == SEEK_END)
    memfile->pos = memfile->length + pos;

  if (memfile->pos > memfile->length)
    memfile->pos = memfile->length;
}

int memtell(unsigned int handle)
{
  MEMFILE *memfile = (MEMFILE *)handle;
  return(memfile->pos);
}
