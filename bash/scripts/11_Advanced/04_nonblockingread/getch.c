#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    int ch;
    int flags = fcntl(STDIN_FILENO, F_GETFL);
    if (flags == -1) return -1; // error
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

    ch = fgetc(stdin);
    if (ch == EOF) return -1;
    if (ch == -1) return -1;
    printf("%c", ch);
    return 0;
}

