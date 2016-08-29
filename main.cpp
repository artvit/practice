#include <unistd.h>
#include <cstdio>
#include <sys/wait.h>
#include <cstdlib>

#include "synchronizing.h"
#include "find_duplicates.h"
#include "compare_files.h"

int main(int argc, char** argv)
{
    if (argc != 4) {
        perror("Not enough arguments");
        exit(-1);
    }
    int processnum = atoi(argv[3]);
    init(4);
    close(task_pipe[0]);
    int pid = fork();
    if (pid == 0) {
        close(result_pipe[0]);
        FindDuplicates(argv[1], argv[2]);
        close(result_pipe[1]);
        close(task_pipe[1]);
        exit(0);
    } else if (pid > 0) {
        close(result_pipe[1]);
        close(task_pipe[1]);
        compare_result cr;
        int counter = 0;
        while (read(result_pipe[0], &cr, sizeof(compare_result)) > 0) {
            if (cr.result) {
                printf("pid: %d\nfile 1: %s\nbytes: %d\nfile 2: %s\nbytes: %d\n\n",
                       cr.pid, cr.file1, cr.bytesInFile1, cr.file2, cr.bytesInFile2);
                ++counter;
            }
        }
        if (counter == 0)
            printf("No duplicates found");
        close(result_pipe[0]);
        wait(NULL);
    }
    destroy();
}

