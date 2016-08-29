#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstdlib>
#include <cstring>

#include "compare_files.h"

bool CompareFiles(compare_result& cr)
{
    struct stat stat1, stat2;
    stat(cr.file1, &stat1);
    stat(cr.file2, &stat2);
    cr.bytesInFile1 = (int) stat1.st_size;
    cr.bytesInFile2 = (int) stat2.st_size;
    cr.pid = getpid();
    cr.result = false;

    if (cr.bytesInFile1 != cr.bytesInFile2)
        return false;

    int fdFile1  = open(cr.file1, O_RDONLY);
    int fdFile2	= open(cr.file2, O_RDONLY);

    if (fdFile1 == -1 || fdFile2 == -1)
        return false;

    int chunkSize = 1024, bytesRead;
    char *buffer1 = new char[chunkSize];
    char *buffer2 = new char[chunkSize];

    while (true) {
        bytesRead = read(fdFile1, buffer1, chunkSize);
        if (bytesRead <= 0)
            break;
        bytesRead = read(fdFile2, buffer2, bytesRead);

        if (strcmp(buffer1, buffer2)) {
            delete[] buffer1;
            delete[] buffer2;
            return false;
        }
    }
    cr.result = true;

    delete[] buffer1;
    delete[] buffer2;

    return true;
}

