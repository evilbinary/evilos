#ifndef _SEM_H
#define _SEM_H

#include <queue.h>
#include <type.h>
#include <task.h>

struct task_t {
    int data;
    struct task_struct *task;
    SIMPLEQ_ENTRY(task_t) entries;
};

// declare type
SIMPLEQ_HEAD(taskqueue_t, task_t);
// define variant

struct semaphore_t{
    e32 count;
    e32 mutex;
    struct taskqueue_t wq;
};

void sem_init(struct semaphore_t* sem,int val);
void sem_wait(struct semaphore_t* sem);
void sem_post(struct semaphore_t* sem);

#endif
