/* realpath: a trivial shell interface to the realpath(3) function call.  */

#include <sys/param.h>
#include <stdlib.h>
#include <stdio.h>

main(int argc, char *argv[])
{

    char pathbuf[PATH_MAX];

    if (argc != 2) {
	fprintf(stderr, "Usage: realpath <filename>\n");
	exit(-1);
    }

    if (!realpath(argv[1], pathbuf)) {
	fprintf(stderr, "Failed.\n");
	perror(argv[1]);
	exit(1);
    }
    printf("%s\n", pathbuf);
    exit(0);
}
