/****************************************************************************
 *                                                                          *
 * Filename: main.c                                                         *
 *                                                                          *
 * Purpose : Standard C sample for Pelles C for Windows.                    *
 *                                                                          *
 *           Word frequency count utility.                                  *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-09-10  Created                                              *
 *                                                                          *
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXWORD 20
#define LETTER 'a'
#define DIGIT '0'

struct tnode {
    char *word;             /* points to the text */
    int count;              /* number of occurrences */
    struct tnode *left;     /* left child */
    struct tnode *right;    /* right child */
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
static int getword(FILE *fd, char *w, int lim)
{
    int c, t;

    if (type(c = *w++ = getc(fd)) != LETTER)
    {
        *w = '\0';
        return c;
    }
    while (--lim > 0)
    {
        t = type(c = *w++ = getc(fd));
        if (t != LETTER && t != DIGIT)
        {
            ungetc(c, fd);
            break;
        }
    }
    *(w-1) = '\0';
    return LETTER;
}

/* save string s somewhere */
static char *strsave(char *s)
{
    char *p;

    if ((p = malloc(strlen(s)+1)) != NULL)
        strcpy(p, s);

    return p;
}

/* install w at or below p */
static struct tnode *tree(struct tnode *p, char *w)
{
    int cond;

    if (p == NULL)  /* a new word has arrived */
    {
        p = malloc(sizeof *p);  /* make a new node */
        p->word = strsave(w);
        p->count = 1;
        p->left = p->right = NULL;
    }
    else if ((cond = strcmp(w, p->word)) == 0)
        p->count++;  /* repeated word */
    else if (cond < 0)  /* lower goes into left subtree */
        p->left = tree(p->left, w);
    else  /* greater into right subtree */
        p->right = tree(p->right, w);

    return p;
}

/* print tree p recursively */
static void treeprint(struct tnode *p)
{
    if (p != NULL)
    {
        treeprint(p->left);
        printf("%5d %s\n", p->count, p->word);
        treeprint(p->right);
    }
}

/* entry point */
int main(int argc, char *argv[])
{
    struct tnode *root;
    char word[MAXWORD];
    int t;
    FILE *fd;

    if (argc != 2)
        error("usage: wordfreq <textfile>", NULL);

    if ((fd = fopen(argv[1], "r")) == NULL)
        error("wordfreq: can't open '%s'", argv[1]);

    root = NULL;
    while ((t = getword(fd, word, MAXWORD)) != EOF)
        if (t == LETTER)
            root = tree(root, word);

    fclose(fd);

    treeprint(root);
    return 0;
}

