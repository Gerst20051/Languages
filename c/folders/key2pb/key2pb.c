/*
    Copyright 2004,2005,2006,2007,2008,2009 Luigi Auriemma

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

    http://www.gnu.org/licenses/gpl-2.0.txt
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include "pbmd5.cpp"

typedef uint8_t     u8;
typedef uint16_t    u16;
typedef uint32_t    u32;



#define VER         "0.4"



u32     scan[] = {
        0x3b9ac617, 0x3b9ac616, 0x054c4085, 0x00b684a3, 0x8d72af09, 0x000006bf, 0xbed3dd55, 0xb2a705c9,
        0x8d1007df, 0xbedf3d6b, 0xb518aa83, 0x000006ad, 0xbe6d7c8f, 0xb2acb5d4, 0x000006be, 0xbece2d4a,
        0x00051a56, 0x000006bd, 0xbec87d3f, 0xb32426bb, 0x000005f3, 0xba4b9491, 0xb2f0f658, 0x000006b4,
        0xbe954cdc, 0x987570ee, 0xb4a6e9a7, 0x000006c2, 0xbee4ed76, 0x000017b5, 0xb529baa4, 0xb35d0729,
        0xb0e5b264, 0x000005f5, 0xba56f4a7, 0xb3aca7c3, 0x000006c0, 0xbed98d60, 0x8b1006f4, 0x000006c1,
        0 };



void do_md5(u8 *key, u32 seed);
u8 *do_md5x(u8 *data, int len, int seed);
int get_num(u8 *str, int *len);



int main(int argc, char *argv[]) {
    int     i,
            j;
    u8      *key1,
            *key2,
            *p1,
            *p2,
            *k;

    setbuf(stdout, NULL);

    fputs("\n"
        "Cdkey to Punkbuster GUID tester "VER"\n"
        "by Luigi Auriemma\n"
        "e-mail: aluigi@autistici.org\n"
        "web:    aluigi.org\n"
        "\n", stdout);

    if(argc < 2) {
        printf("\n"
            "Usage: %s <cdkey> [seed1] ... [seedN]\n"
            "\n"
            "Note: this tool will compute all the MD5 hashes available for each game using\n"
            "      both the cdkey as is and in lower case and without the '-' char\n"
            "      the best way to know the what is the exact seed used by your specific\n"
            "      game is trying to use your cdkey and comparing the hashes with your\n"
            "      pb_g_guid and pb_guid.\n"
            "      Usually the most used seeds are 0x00b684a3 and 0x00051a56 which are used\n"
            "      combined in the recent games, in this case use:\n"
            "        key2pb CDKEY 0x00b684a3 0x00051a56\n"
            "\n", argv[0]);
        exit(1);
    }

    key1 = strdup(argv[1]);
    key2 = strdup(argv[1]);

    for(p1 = key1, p2 = key2; *p1; p1++) {
        if(*p1 == '-') continue;
        if((*p1 >= 'A') && (*p1 <= 'Z')) {
            *p2++ = *p1 + 32;
        } else {
            *p2++ = *p1;
        }
    }
    *p2 = 0;

    if(argc > 2) {
        for(i = 0; i < 2; i++) {
            k = key1;
            if(i == 1) k = key2;

            printf("- GUID of cdkey %s: ", k);
            k = do_md5x(k, strlen(k), get_num(argv[2], NULL));
            for(j = 3; j < argc; j++) {
                k = do_md5x(k, 32, get_num(argv[j], NULL));
            }
            printf("%s\n", k);
        }
        return(0);
    }

    for(i = 0; i < 2; i++) {
        k = key1;
        if(i == 1) k = key2;

        printf("\n- GUIDs of cdkey %s:\n", k);

        for(j = 0; scan[j]; j++) {
            do_md5(k, scan[j]);
        }
    }

    return(0);
}



void do_md5(u8 *key, u32 seed) {
    MD5_CTX     ctx;
    int         i;
    char        hash[33];
    static const char   hex[] = "0123456789abcdef";

    MD5Init(&ctx, seed);
    MD5Update(&ctx, key, strlen(key));
    MD5Final(&ctx);

    for(i = 0; i < 16; i++) {
        hash[i << 1]       = hex[ctx.digest[i] >> 4];
        hash[(i << 1) + 1] = hex[ctx.digest[i] & 15];
    }
    hash[i << 1] = 0;

    printf("  %08x %s\n", seed, hash);
}



u8 *do_md5x(u8 *data, int len, int seed) {
    MD5_CTX     ctx;
    int     i;
    u8      *p;
    static u8   ret[33];
    static const u8 hex[] = "0123456789abcdef";

    MD5Init(&ctx, seed);
    MD5Update(&ctx, data, len);
    MD5Final(&ctx);

    p = ret;
    for(i = 0; i < 16; i++) {
        *p++ = hex[ctx.digest[i] >> 4];
        *p++ = hex[ctx.digest[i] & 15];
    }
    *p = 0;
    return(ret);
}



int get_num(u8 *str, int *len) {
    int     num,
            tmp;

    if(!len) len = &tmp;
    *len = 0;
    if(!str[0]) return(0);
    if((strlen(str) > 2) && (str[0] == '0') && (tolower(str[1]) == 'x')) {
        sscanf(str + 2, "%x%n", &num, len);
    } else {
        sscanf(str, "%d%n", &num, len);
    }
    return(num);
}


