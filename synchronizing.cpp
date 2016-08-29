#include <fcntl.h>
#include <semaphore.h>
#include <cstdlib>
#include <stdio.h>
#include <unistd.h>

#include "compare_files.h"

sem_t* semaphore;
int result_pipe[2];
int task_pipe[2];

void start_worker();

void init(int value)
{
    semaphore = sem_open("file_duplic_semaphore17", O_CREAT, 0644, 1);
    if (semaphore == SEM_FAILED) {
        perror("Unable to create semaphore");
        sem_close(semaphore);
        exit(-1);
    }
    if(pipe(result_pipe)) {
        perror("Unable to create pipes");
        sem_close(semaphore);
        exit(-1);
    }
    if(pipe(task_pipe)) {
        perror("Unable to create pipes");
        sem_close(semaphore);
        exit(-1);
    }
    for (int i = 0; i < value; ++i) {
        start_worker();
    }
}

void destroy()
{
    if (sem_close(semaphore)) {
        perror("Cannot close semaphore");
    }
}

void start_worker()
{
    int pid = fork();
    if (pid == 0){
        close(task_pipe[1]);
        compare_result cr;
        while (true) {
            sem_wait(semaphore);
            int bytes_read = (int) read(task_pipe[0], &cr, sizeof(compare_result));
            sem_post(semaphore);
            if (bytes_read > 0) {
                CompareFiles(cr);
                write(result_pipe[1], &cr, sizeof(compare_result));
            }
            else
                break;
        }
        close(task_pipe[0]);
        exit(0);
    } else if (pid < 0) {
        perror("Cannot fork process");
        sleep(10);
    }
}