/*
 * Simple test driver for RSA Data Security, Inc. MD5 message-digest algorithm
 * (C and X86 assembly version for comparison...)
 */

#include <string.h>
#include <stdio.h>

/* C implementation of MD5 algorithm */
extern void __cdecl c_md5_test(unsigned char [16], unsigned char *, unsigned int);
/* X86 assembly implementation of MD5 algorithm */
extern void __cdecl asm_md5_test(unsigned char [16], unsigned char *, unsigned int);

/* Some "random" data... */
unsigned char data[] = {
    "aaaaaaaaaa" "aaaaaaaaaa" "aaaaaaaaaa" "aaaaaaaaaa" "aaaaaaaaaa"
    "bbbbbbbbbb" "bbbbbbbbbb" "bbbbbbbbbb" "bbbbbbbbbb" "bbbbbbbbbb"
    "cccccccccc" "cccccccccc" "cccccccccc" "cccccccccc" "cccccccccc"
    "dddddddddd" "dddddddddd" "dddddddddd" "dddddddddd" "dddddddddd"
    "eeeeeeeeee" "eeeeeeeeee" "eeeeeeeeee" "eeeeeeeeee" "eeeeeeeeee"
    "ffffffffff" "ffffffffff" "ffffffffff" "ffffffffff" "ffffffffff"
    "gggggggggg" "gggggggggg" "gggggggggg" "gggggggggg" "gggggggggg"
    "hhhhhhhhhh" "hhhhhhhhhh" "hhhhhhhhhh" "hhhhhhhhhh" "hhhhhhhhhh"
    "aaaaaaaaaa" "aaaaaaaaaa" "aaaaaaaaaa" "aaaaaaaaaa" "aaaaaaaaaa"
    "bbbbbbbbbb" "bbbbbbbbbb" "bbbbbbbbbb" "bbbbbbbbbb" "bbbbbbbbbb"
    "cccccccccc" "cccccccccc" "cccccccccc" "cccccccccc" "cccccccccc"
    "dddddddddd" "dddddddddd" "dddddddddd" "dddddddddd" "dddddddddd"
    "eeeeeeeeee" "eeeeeeeeee" "eeeeeeeeee" "eeeeeeeeee" "eeeeeeeeee"
    "ffffffffff" "ffffffffff" "ffffffffff" "ffffffffff" "ffffffffff"
    "gggggggggg" "gggggggggg" "gggggggggg" "gggggggggg" "gggggggggg"
    "hhhhhhhhhh" "hhhhhhhhhh" "hhhhhhhhhh" "hhhhhhhhhh" "hhhhhhhhhh"
    "aaaaaaaaaa" "aaaaaaaaaa" "aaaaaaaaaa" "aaaaaaaaaa" "aaaaaaaaaa"
    "bbbbbbbbbb" "bbbbbbbbbb" "bbbbbbbbbb" "bbbbbbbbbb" "bbbbbbbbbb"
    "cccccccccc" "cccccccccc" "cccccccccc" "cccccccccc" "cccccccccc"
    "dddddddddd" "dddddddddd" "dddddddddd" "dddddddddd" "dddddddddd"
    "eeeeeeeeee" "eeeeeeeeee" "eeeeeeeeee" "eeeeeeeeee" "eeeeeeeeee"
    "ffffffffff" "ffffffffff" "ffffffffff" "ffffffffff" "ffffffffff"
    "gggggggggg" "gggggggggg" "gggggggggg" "gggggggggg" "gggggggggg"
    "hhhhhhhhhh" "hhhhhhhhhh" "hhhhhhhhhh" "hhhhhhhhhh" "hhhhhhhhhh"
    "aaaaaaaaaa" "aaaaaaaaaa" "aaaaaaaaaa" "aaaaaaaaaa" "aaaaaaaaaa"
    "bbbbbbbbbb" "bbbbbbbbbb" "bbbbbbbbbb" "bbbbbbbbbb" "bbbbbbbbbb"
    "cccccccccc" "cccccccccc" "cccccccccc" "cccccccccc" "cccccccccc"
    "dddddddddd" "dddddddddd" "dddddddddd" "dddddddddd" "dddddddddd"
    "eeeeeeeeee" "eeeeeeeeee" "eeeeeeeeee" "eeeeeeeeee" "eeeeeeeeee"
    "ffffffffff" "ffffffffff" "ffffffffff" "ffffffffff" "ffffffffff"
    "gggggggggg" "gggggggggg" "gggggggggg" "gggggggggg" "gggggggggg"
    "hhhhhhhhhh" "hhhhhhhhhh" "hhhhhhhhhh" "hhhhhhhhhh" "hhhhhhhhhh"
    "aaaaaaaaaa" "aaaaaaaaaa" "aaaaaaaaaa" "aaaaaaaaaa" "aaaaaaaaaa"
    "bbbbbbbbbb" "bbbbbbbbbb" "bbbbbbbbbb" "bbbbbbbbbb" "bbbbbbbbbb"
    "cccccccccc" "cccccccccc" "cccccccccc" "cccccccccc" "cccccccccc"
    "dddddddddd" "dddddddddd" "dddddddddd" "dddddddddd" "dddddddddd"
    "eeeeeeeeee" "eeeeeeeeee" "eeeeeeeeee" "eeeeeeeeee" "eeeeeeeeee"
    "ffffffffff" "ffffffffff" "ffffffffff" "ffffffffff" "ffffffffff"
    "gggggggggg" "gggggggggg" "gggggggggg" "gggggggggg" "gggggggggg"
    "hhhhhhhhhh" "hhhhhhhhhh" "hhhhhhhhhh" "hhhhhhhhhh" "hhhhhhhhhh"
    "aaaaaaaaaa" "aaaaaaaaaa" "aaaaaaaaaa" "aaaaaaaaaa" "aaaaaaaaaa"
    "bbbbbbbbbb" "bbbbbbbbbb" "bbbbbbbbbb" "bbbbbbbbbb" "bbbbbbbbbb"
    "cccccccccc" "cccccccccc" "cccccccccc" "cccccccccc" "cccccccccc"
    "dddddddddd" "dddddddddd" "dddddddddd" "dddddddddd" "dddddddddd"
    "eeeeeeeeee" "eeeeeeeeee" "eeeeeeeeee" "eeeeeeeeee" "eeeeeeeeee"
    "ffffffffff" "ffffffffff" "ffffffffff" "ffffffffff" "ffffffffff"
    "gggggggggg" "gggggggggg" "gggggggggg" "gggggggggg" "gggggggggg"
    "hhhhhhhhhh" "hhhhhhhhhh" "hhhhhhhhhh" "hhhhhhhhhh" "hhhhhhhhhh"
    "aaaaaaaaaa" "aaaaaaaaaa" "aaaaaaaaaa" "aaaaaaaaaa" "aaaaaaaaaa"
    "bbbbbbbbbb" "bbbbbbbbbb" "bbbbbbbbbb" "bbbbbbbbbb" "bbbbbbbbbb"
    "cccccccccc" "cccccccccc" "cccccccccc" "cccccccccc" "cccccccccc"
    "dddddddddd" "dddddddddd" "dddddddddd" "dddddddddd" "dddddddddd"
    "eeeeeeeeee" "eeeeeeeeee" "eeeeeeeeee" "eeeeeeeeee" "eeeeeeeeee"
    "ffffffffff" "ffffffffff" "ffffffffff" "ffffffffff" "ffffffffff"
    "gggggggggg" "gggggggggg" "gggggggggg" "gggggggggg" "gggggggggg"
    "hhhhhhhhhh" "hhhhhhhhhh" "hhhhhhhhhh" "hhhhhhhhhh" "hhhhhhhhhh"
    "aaaaaaaaaa" "aaaaaaaaaa" "aaaaaaaaaa" "aaaaaaaaaa" "aaaaaaaaaa"
    "bbbbbbbbbb" "bbbbbbbbbb" "bbbbbbbbbb" "bbbbbbbbbb" "bbbbbbbbbb"
    "cccccccccc" "cccccccccc" "cccccccccc" "cccccccccc" "cccccccccc"
    "dddddddddd" "dddddddddd" "dddddddddd" "dddddddddd" "dddddddddd"
    "eeeeeeeeee" "eeeeeeeeee" "eeeeeeeeee" "eeeeeeeeee" "eeeeeeeeee"
    "ffffffffff" "ffffffffff" "ffffffffff" "ffffffffff" "ffffffffff"
    "gggggggggg" "gggggggggg" "gggggggggg" "gggggggggg" "gggggggggg"
    "hhhhhhhhhh" "hhhhhhhhhh" "hhhhhhhhhh" "hhhhhhhhhh" "hhhhhhhhhh"
};

/* hmm... */
int main(void)
{
    unsigned char c_digest[16];
    unsigned char asm_digest[16];

    /* C digest of data */
    c_md5_test(c_digest, data, sizeof(data));
    /* Assembly digest of data */
    asm_md5_test(asm_digest, data, sizeof(data));

    /* Are they the same? (they should be...) */
    if (memcmp(c_digest, asm_digest, sizeof(c_digest)) == 0)
    {
        puts("Say what?! It works?! The C and assembly digests are the same...!");
        return 0;
    }
    else
    {
        puts("Bummer... something is wrong... terribly wrong...");
        for (size_t i = 0; i < sizeof(c_digest); i++)
            printf("%zX: %02X - %02X\n", i, c_digest[i], asm_digest[i]);
        return 1;
    }
}
