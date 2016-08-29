#ifndef PRACTIE_SYNCHRONIZING_H
#define PRACTIE_SYNCHRONIZING_H

#include <semaphore.h>

extern sem_t* semaphore;
extern int result_pipe[2];
extern int task_pipe[2];

void init(int value);
void destroy();
void start_worker();

#endif
