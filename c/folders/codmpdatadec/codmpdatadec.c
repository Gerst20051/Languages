/*
    Copyright 2009 Luigi Auriemma

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
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "md4.h"

#ifdef WIN32
/*
#include <windows.h>
int regkey(HKEY hKey, LPCTSTR lpSubKey, LPTSTR lpValueName, unsigned char *buff, DWORD len) {
    HKEY    key;
    DWORD   type;

    buff[0] = 0;
    if(RegOpenKeyEx(hKey, lpSubKey, 0, KEY_ALL_ACCESS, &key) != ERROR_SUCCESS) {
        return(-1);
    }
    if(RegQueryValueEx(key, lpValueName, NULL, &type, buff, &len) != ERROR_SUCCESS) {
        RegCloseKey(key);
        return(-1);
    }
    RegCloseKey(key);
    return(0);
}
*/
#endif

typedef uint8_t     u8;
typedef uint32_t    u32;



#define VER         "0.1.1a"



typedef struct {
    u8      sign[4];
    u32     seed;
    u8      hash[16];
    u8      data[0x2104];
} iwm_t;

static u8 cdkey_help[] =
            "  the cdkey must be bracketsless like the one located in the registry:\n"
            "   HKEY_LOCAL_MACHINE\\SOFTWARE\\Activision\\Call of Duty 4\\codkey\n"
            "   HKEY_LOCAL_MACHINE\\SOFTWARE\\Activision\\Call of Duty WAW\\codkey\n"
            "   HKEY_LOCAL_MACHINE\\SOFTWARE\\Activision\\CODWAWbeta\\codkey\n"
            "   and so on...\n"
            "  in Call of Duty 4 MUST be used only the first 16 bytes of the cdkey\n"
            "\n"
            "  examples:\n"
            "   cod4:   codmpdatadec c:\\mpdata c:\\output.txt AAAABBBBCCCCDDDD\n"
            "   codwaw: codmpdatadec c:\\mpdata c:\\output.txt AAAABBBBCCCCDDDDEEEE\n";



void quick_showhex(u8 *msg, u8 *data, int datalen);
void iwm_hash(u8 *key, int keylen, u8 *data, int datalen, u8 *out);
void iwm_key(u8 *cdkey, u32 seed, u8 *key);
void iwm_decrypt(u32 *data, u32 *key);
void iwm_encrypt(u32 *data, u32 *key);
int check_overwrite(u8 *fname);
void std_err(void);



int main(int argc, char *argv[]) {
    FILE    *fd;
    iwm_t   iwm;
    u32     tmp;
    int     encrypt = 0;
    u8      key[16],
            myhash[16],
            *mpdata,
            *fname,
            *cdkey;

    fputs("\n"
        "Call of Duty series mpdata decrypter/encrypter "VER"\n"
        "by Luigi Auriemma\n"
        "e-mail: aluigi@autistici.org\n"
        "web:    aluigi.org\n"
        "\n", stdout);

    if(argc < 4) {
        printf("\n"
            "Usage: %s <input_file> <output_file> <cdkey>\n"
            "\n"
            "the tool automatically decrypts the input file if it's a mpdata/iwm file,\n"
            "otherwise it encrypts the file\n"
            "\n"
            "the mpdata file is available in the same game \"players\" folder in cod4 or in\n"
            "the user's \"Application Data\\Activision\" sub-folders in codwaw\n"
            "\n"
            "%s"
            "\n", argv[0], cdkey_help);
        exit(1);
    }

    mpdata  = argv[1];
    fname   = argv[2];
    cdkey   = argv[3];

    printf("- open file %s\n", mpdata);
    fd = fopen(mpdata, "rb");
    if(!fd) std_err();
    tmp = fread(&iwm, 1, sizeof(iwm_t), fd);
    if(tmp == sizeof(iwm_t)) {
        printf("- activated decryption mode\n");
    } else if(tmp == sizeof(iwm.data)) {
        printf("- activated encryption mode\n");
        encrypt = 1;
        memmove(&iwm.data, &iwm.sign, sizeof(iwm.data));
        memset(iwm.hash, 0, sizeof(iwm.hash));
        memcpy(iwm.sign, "iwm0", sizeof(iwm.sign));
        iwm.seed = time(NULL);  // timeGetTime
    } else {
        printf("\n"
            "Error: the input file has an invalid size which doens't match the encrypted\n"
            "       (%d) or decrypted (%d) mpdata filesize\n", sizeof(iwm_t), sizeof(iwm.data));
        exit(1);
    }
    fclose(fd);

    if(memcmp(iwm.sign, "iwm0", sizeof(iwm.sign))) {
        printf("\nError: the input file is not an iwm0 file (%.4s)\n", iwm.sign);
        exit(1);
    }

    //if(regkey(  // example of how to read the codkey automatically, not implemented due to the various cod games available
        //HKEY_LOCAL_MACHINE, "SOFTWARE\\Activision\\Call of Duty 4", "codkey",
        //cdkey, sizeof(cdkey)) < 0) cdkey[0] = 0;

    printf("- mpdata seed 0x%08x\n", iwm.seed);
    iwm_key(cdkey, iwm.seed, key);
    quick_showhex("- encryption key: ", key, 16);

    if(encrypt) {
        tmp = (*(u32 *)(key + 8) + 0x928d764c) ^ iwm.seed;
        iwm_hash((u8 *)&tmp, 4, iwm.data, sizeof(iwm.data), iwm.hash);
        quick_showhex("- data hash: ", iwm.hash, 16);
        printf("- encrypt\n");
        iwm_encrypt((u32 *)iwm.hash, (u32 *)key);
    } else {
        printf("- decrypt\n");
        iwm_decrypt((u32 *)iwm.hash, (u32 *)key);
        quick_showhex("- data hash: ", iwm.hash, 16);
        tmp = (*(u32 *)(key + 8) + 0x928d764c) ^ iwm.seed;
        iwm_hash((u8 *)&tmp, 4, iwm.data, sizeof(iwm.data), myhash);
        if(memcmp(iwm.hash, myhash, 16)) {
            printf("\n"
                "Error: the hash doesn't match, verify your cdkey and moreover its length!\n"
                "\n"
                "%s"
                "\n", cdkey_help);
            exit(1);
        }
    }

    printf("- create file %s\n", fname);
    if(check_overwrite(fname) < 0) exit(1);
    fd = fopen(fname, "wb");
    if(!fd) std_err();
    if(encrypt) {
        fwrite(&iwm, 1, sizeof(iwm_t), fd);
    } else {
        fwrite(iwm.data, 1, sizeof(iwm.data), fd);  // skip the hash
    }
    fclose(fd);

    printf("- done\n");
    return(0);
}



void quick_showhex(u8 *msg, u8 *data, int datalen) {
    int     i;

    printf("%s", msg);
    for(i = 0; i < datalen; i++) printf("%02x", data[i]);
    printf("\n");
}



void iwm_hash(u8 *key, int keylen, u8 *data, int datalen, u8 *out) {
    md4_context ctx;

    md4_starts(&ctx);
    md4_update(&ctx, key, keylen);
    md4_update(&ctx, data, datalen);
    md4_finish(&ctx, out);
}



void iwm_key(u8 *cdkey, u32 seed, u8 *key) {   // custom md4 hmac
    u32     cod_seed    = 0x1f93ab07;
    int     i;
    u8      codkey[34],
            tmp[16],
            tmp36[16],
            tmp5c[16];

    memset(codkey, ' ', 32);
    codkey[32] = 0;
    codkey[33] = 0;
    if(strlen(cdkey) < 34) {
        strcpy(codkey, cdkey);
    } else {
        strncpy(codkey, cdkey, 34);
    }

    if(strlen(cdkey) > 16) {    // codwaw
        codkey[22] = 0;
    }

    iwm_hash((u8 *)&cod_seed, 4, codkey, 34, tmp);

    for(i = 0; i < 16; i++) {
        tmp36[i] = tmp[i] ^ 0x36;
        tmp5c[i] = tmp[i] ^ 0x5c;
    }

    iwm_hash(tmp36, 16, (u8 *)&seed, 4, tmp);
    iwm_hash(tmp5c, 16, tmp, 16, key);
}



void iwm_decrypt(u32 *data, u32 *key) { // custom tea
    u32     delta,
            n,
            t;
    int     i;

    delta = 0xb54cda56;
    do {
        n = data[0];
        for(i = 0x844; i; i--) {
            t = data[i - 1];
            data[i] -=
                (((t >> 5) ^ (n << 2)) + ((t << 4) ^ (n >> 3))) ^
                ((key[(i ^ (delta >> 2)) & 3] ^ t) + (delta ^ n));
            n = data[i];
        }
        t = data[0x844];
        data[0] -=
            (((t >> 5) ^ (n << 2)) + ((t << 4) ^ (n >> 3))) ^
            ((key[(i ^ (delta >> 2)) & 3] ^ t) + (delta ^ n));
        delta += 0x61c88647;
    } while(delta);
}



void iwm_encrypt(u32 *data, u32 *key) { // custom tea
    u32     delta,
            n,
            t;
    int     i,
            j;

    delta = 0;
    for(j = 6; j; j--) {
        delta -= 0x61c88647;
        n = data[0x844];
        for(i = 0; i < 0x844; i++) {
            t = data[i + 1];
            data[i] +=
                (((n >> 5) ^ (t << 2)) + ((n << 4) ^ (t >> 3))) ^
                ((key[(i ^ (delta >> 2)) & 3] ^ n) + (delta ^ t));
            n = data[i];
        }
        t = data[0];
        data[0x844] +=
            (((n >> 5) ^ (t << 2)) + ((n << 4) ^ (t >> 3))) ^
            ((key[(i ^ (delta >> 2)) & 3] ^ n) + (delta ^ t));
    }
}



int check_overwrite(u8 *fname) {
    FILE    *fd;
    u8      ans[16];

    fd = fopen(fname, "rb");
    if(!fd) return(0);
    fclose(fd);
    printf("  the file already exists, do you want to overwrite it (y/N)? ");
    fgets(ans, sizeof(ans), stdin);
    if(tolower(ans[0]) != 'y') return(-1);
    return(0);
}



void std_err(void) {
    perror("\nError");
    exit(1);
}


