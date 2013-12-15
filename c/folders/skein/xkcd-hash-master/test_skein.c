#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "skein.h"

void print_bytes(u08b_t *bytes, int num_bytes)
{
  int i = 0;
  for (i = 0; i < num_bytes; i++) {
    printf("%02x", bytes[i]);
  }
  printf("\n");
}


// *************************************************************************

u08b_t desired[] =
  "\x5b\x4d\xa9\x5f\x5f\xa0\x82\x80\xfc\x98\x79\xdf\x44\xf4\x18\xc8\xf9"
  "\xf1\x2b\xa4\x24\xb7\x75\x7d\xe0\x2b\xbd\xfb\xae\x0d\x4c\x4f\xdf\x93"
  "\x17\xc8\x0c\xc5\xfe\x04\xc6\x42\x90\x73\x46\x6c\xf2\x97\x06\xb8\xc2"
  "\x59\x99\xdd\xd2\xf6\x54\x0d\x44\x75\xcc\x97\x7b\x87\xf4\x75\x7b\xe0"
  "\x23\xf1\x9b\x8f\x40\x35\xd7\x72\x28\x86\xb7\x88\x69\x82\x6d\xe9\x16"
  "\xa7\x9c\xf9\xc9\x4c\xc7\x9c\xd4\x34\x7d\x24\xb5\x67\xaa\x3e\x23\x90"
  "\xa5\x73\xa3\x73\xa4\x8a\x5e\x67\x66\x40\xc7\x9c\xc7\x01\x97\xe1\xc5"
  "\xe7\xf9\x02\xfb\x53\xca\x18\x58\xb6";

u08b_t hashval[128];

int hashit(const char *msg, int nbytes)
{
  Skein1024_Ctxt_t ctx;

  Skein1024_Init(&ctx, 1024);
  Skein1024_Update(&ctx, msg, nbytes);
  Skein1024_Final(&ctx, hashval);

  int res = 0;
  for (int i = 0; i < 128; ++i) {
    unsigned char x = hashval[i] ^ desired[i];
    for (int j = 0; j < 8; ++j)
      res += !!(x & (1<<j));
  }
  return res;
}

// *************************************************************************

int randr(int min, int max) {
  double scaled = (double)rand() / RAND_MAX;
  return (max - min + 1)*scaled + min;
}

int main(int argc, char **argv)
{
  /*const char tests[] = "(";*/
  /*printf("%d\n", hashit(tests, strlen(tests)));*/
  /*for (int i = 0; i < 128; ++i)*/
    /*printf("%02x", hashval[i]);*/
  /*printf("\n");*/
  /*return 0;*/

  srand(time(NULL));

  int mini = 1000000;
  char s[100];
  for (;;) {
    int len = 10;
    for (int i = 0; i < len; ++i)
      /*s[i] = randr(32, 126);*/
      s[i] = randr(97, 122);
    s[len] = 0;
    int res = hashit(s, len);
    if (res < mini) {
      printf("New min: %d %s\n", res, s);
      fflush(stdout);
      mini = res;
    }
  }
}
