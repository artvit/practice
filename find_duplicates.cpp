#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <semaphore.h>

#include "synchronizing.h"
#include "compare_files.h"

int FindFileDuplicates(char* file, char* dirpath)
{
    DIR *dir;
    struct dirent *dp;
    struct dirent *result;
    struct stat statp;

    char absoluteFilePath[255];

    if ((dir = opendir(dirpath))== NULL) {
        return -1;
    }
    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") && strcmp(dp->d_name,".."))
        {
            strcpy(absoluteFilePath, dirpath);
            strcat(absoluteFilePath, "/");
            strcat(absoluteFilePath, dp->d_name);
            stat(absoluteFilePath, &statp);

            if (S_ISDIR(statp.st_mode))	{
                FindFileDuplicates(file, absoluteFilePath);
            }
            else if (S_ISREG(statp.st_mode)) {
                if (strcmp(file, absoluteFilePath)) {
                    compare_result cr;
                    strcpy(cr.file1, file);
                    strcpy(cr.file2, absoluteFilePath);
                    write(task_pipe[1], &cr, sizeof(compare_result));
                }
            }
        }
    }
    closedir(dir);
    return 0;
}

int FindDuplicates(char* dir1path, char* dir2path)
{
    DIR *dir1;
    struct dirent *dp1;
    struct stat statp;

    char absoluteFile1Path[255];

    if ((dir1 = opendir(dir1path))== NULL) {
        perror("Failed to open directory 1");
        return -1;
    }
    while ((dp1 = readdir(dir1)) != NULL) {
        if (strcmp(dp1->d_name, ".") && strcmp(dp1->d_name,"..")) {
            strcpy(absoluteFile1Path, dir1path);
            strcat(absoluteFile1Path, "/");
            strcat(absoluteFile1Path, dp1->d_name);
            stat(absoluteFile1Path, &statp);
            if (S_ISDIR(statp.st_mode))	{
                FindDuplicates(absoluteFile1Path, dir2path);
            }
            else if (S_ISREG(statp.st_mode)) {
                FindFileDuplicates(absoluteFile1Path, dir2path);
            }

        }
    }
    closedir(dir1);
    return 0;
}