/****************************************************************************
 *                                                                          *
 * Filename: main.c                                                         *
 *                                                                          *
 * Purpose : Standard C sample for Pelles C for Windows (4.0 or better).    *
 *                                                                          *
 *           Word frequency count utility v2: use functions from search.h.  *
 *                                                                          *
 * History : Date      Reason                                               *
 *           06-11-13  Created                                              *
 *                                                                          *
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <search.h>  /* use _tsearch(), _twalk() and _tdelete() */

#define MAXWORD 20
#define LETTER 'a'
#define DIGIT '0'

/* holds our private data in the binary tree */
struct wordnode {
    char *word;     /* points to the text */
    int count;      /* number of occurrences */
};

/* print error message and die */
static void error(char *msg, char *arg)
{
    printf(msg, arg);
    printf("\n");
    exit(EXIT_FAILURE);
}

/* return type of ASCII character */
static int type(int c)
{
    if (isalpha(c))
        return LETTER;
    else if (isdigit(c))
        return DIGIT;
    else
        return c;
}

/* get next word from input */
static int getword(FILE *fd, char *bufp, int buflim)
{
    int c, t;

    if (type(c = *bufp++ = getc(fd)) != LETTER)
    {
        *bufp = '\0';
        return c;
    }
    while (--buflim > 0)
    {
        t = type(c = *bufp++ = getc(fd));
        if (t != LETTER && t != DIGIT)
        {
            ungetc(c, fd);
            break;
        }
    }
    *(bufp-1) = '\0';
    return LETTER;
}

/* save string s somewhere */
static char *strsave(char *s)
{
    char *bufp;

    if ((bufp = malloc(strlen(s)+1)) != NULL)
        strcpy(bufp, s);

    return bufp;
}

/* compare two nodes for the _tsearch or _tdelete function */
static int tcompare(const void *node1, const void *node2)
{
    return strcmp(
        ((const struct wordnode *)node1)->word,
        ((const struct wordnode *)node2)->word);
}

/* print information for the _twalk function */
static void tvisit(const void *node, _VISIT order, int level)
{
    /* extract our data from the current node */
    const struct wordnode *wordnode = *(const struct wordnode **)node;

    /* print number of occurrences, and the text */
    if (order == _postorder || order == _leaf)
        printf("%5d %s\n", wordnode->count, wordnode->word);
}

/* entry point */
int main(int argc, char *argv[])
{
    void *root;
    char word[MAXWORD];
    int t;
    FILE *fd;

    if (argc != 2)
        error("usage: wordfreq <textfile>", NULL);

    if ((fd = fopen(argv[1], "r")) == NULL)
        error("wordfreq: can't open '%s'", argv[1]);

    root = NULL;
    while ((t = getword(fd, word, MAXWORD)) != EOF)
    {
        if (t == LETTER)
        {
            struct wordnode *wordnode;

            /* allocate a new node for the text and number of occurences */
            if ((wordnode = malloc(sizeof(*wordnode))) != NULL)
            {
                void *node;

                /* Initialize the new node */
                wordnode->count = 1;
                wordnode->word = strsave(word);

                /* Insert the node into the tree (if new, else get pointer to existing node) */
                node = _tsearch((void *)wordnode, &root, tcompare);
                if (node != NULL)
                {
                    /* Extract our data from the returned node */
                    struct wordnode *tempnode = *(struct wordnode **)node;
                    /* Same node as we just created above? */
                    if (tempnode != wordnode)
                    {
                        /* No! It must have been inserted before! */
                        tempnode->count++;
                        free(wordnode->word);
                        free(wordnode);
                    }
                }
            }
        }
    }

    fclose(fd);

    /* print the tree (see tvisit function) */
    _twalk(root, tvisit);

    /* free the whole tree */
    while (root)
    {
        struct wordnode *wordnode = *(struct wordnode **)root;
        _tdelete(wordnode, &root, tcompare);
        free(wordnode->word);
        free(wordnode);
    }

    return 0;
}
