/****************************************************************************
 *                                                                          *
 * Filename: dirlist.c                                                      *
 *                                                                          *
 * Purpose : Standard C sample for Pelles C for Windows.                    *
 *                                                                          *
 *           Displays information about files. Uses Pelles C extensions.    *
 *                                                                          *
 * History : Date      Reason                                               *
 *           02-08-26  Created                                              *
 *                                                                          *
 ****************************************************************************/

#include <stdio.h>
#include <io.h>
#include <time.h>

int main(int argc, char **argv)
{
    char *spec = (argc > 1) ? argv[1] : "*.*";
    struct _finddata_t fd;
    long handle;

    handle = _findfirst(spec, &fd);
    if (handle == -1)
    {
        printf("Unable to find any files matching the selection \"%s\"\n", spec);
        return 1;
    }

    do
    {
        printf("%10u  %.*s  %c%c%c%c%c  %s\n",
            fd.size,
            24, ctime(&fd.time_write),
            (fd.attrib & _A_SUBDIR) ? 'd' : '-',
            (fd.attrib & _A_RDONLY) ? 'r' : '-',
            (fd.attrib & _A_HIDDEN) ? 'h' : '-',
            (fd.attrib & _A_SYSTEM) ? 's' : '-',
            (fd.attrib & _A_ARCH) ?   'a' : '-',
            fd.name);
    } while (_findnext(handle, &fd) == 0);
    _findclose(handle);

    return 0;
}

