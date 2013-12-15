/****************************************************************************
 *                                                                          *
 * Filename: main.c                                                         *
 *                                                                          *
 * Purpose : Standard C sample for Pelles C for Windows.                    *
 *                                                                          *
 *           Simple file copy utility.                                      *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-09-10  Created                                              *
 *                                                                          *
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>

/* print error message and die */
static void error(char *msg, char *arg)
{
    printf(msg, arg);
    printf("\n");
    exit(EXIT_FAILURE);
}

/* entry point */
int main(int argc, char *argv[])
{
    FILE *fr, *fw;
    int c;

    if (argc != 3)
        error("usage: filecopy <sourcefile> <targetfile>", NULL);

    if ((fr = fopen(argv[1], "rb")) == NULL)
        error("filecopy: can't open '%s'", argv[1]);

    if ((fw = fopen(argv[2], "wb")) == NULL)
        error("filecopy: can't create '%s'", argv[2]);

    while ((c = fgetc(fr)) != EOF)
        if (fputc(c, fw) == EOF)
            error("filecopy: write error", NULL);

    /* fclose(fw); */
    /* fclose(fr); */

    return 0;
}




