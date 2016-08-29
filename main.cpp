#include <unistd.h>
#include <cstdio>
#include <sys/wait.h>
#include <cstdlib>

#include "synchronizing.h"
#include "find_duplicates.h"
#include "compare_files.h"

int main()
{
    init(4);
    close(task_pipe[0]);
    int pid = fork();
    if (pid == 0) {
        close(result_pipe[0]);
        FindDuplicates("/home/artem/Documents", "/etc");
        close(result_pipe[1]);
        close(task_pipe[1]);
        exit(0);
    } else if (pid > 0) {
        close(result_pipe[1]);
        close(task_pipe[1]);
        compare_result cr;
        while (read(result_pipe[0], &cr, sizeof(compare_result)) > 0) {
            printf("pid: %d\nfile 1: %s\nbytes: %d\nfile 2: %s\nbytes: %d\nresult: %d\n\n",
                   cr.pid, cr.file1, cr.bytesInFile1, cr.file2, cr.bytesInFile2, cr.result);
        }
        close(result_pipe[0]);
        wait(NULL);
    }
    destroy();
}

