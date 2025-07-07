/**
 * Implementation of thread pool.
 */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "threadpool.h"

#define QUEUE_SIZE 10
#define NUMBER_OF_THREADS 3

#define TRUE 1

// this represents work that has to be 
// completed by a thread in the pool
typedef struct 
{
    void (*function)(void *p);
    void *data;
}
task;

sem_t full,empty;
pthread_mutex_t mutex;

// the work queue
task worktodo[QUEUE_SIZE];
int rare,front;

// the worker bee
pthread_t bee[NUMBER_OF_THREADS];

// insert a task into the queue
// returns 0 if successful or 1 otherwise, 
int enqueue(task t) 
{
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);
    worktodo[rare++]=t;
    rare%=QUEUE_SIZE;
    pthread_mutex_unlock(&mutex);
    sem_post(&full);
    return 0;
}

// remove a task from the queue
task dequeue() 
{
    sem_wait(&full);
    pthread_mutex_lock(&mutex);
    task ret=worktodo[front++];
    front%=QUEUE_SIZE;
    pthread_mutex_unlock(&mutex);
    sem_post(&empty);
    return ret;
}

// the worker thread in the thread pool
void *worker(void *param)
{
    // execute the task
    while(1)
    {
        task t=dequeue();
        execute(t.function, t.data);
    }
    pthread_exit(0);
}

/**
 * Executes the task provided to the thread pool
 */
void execute(void (*somefunction)(void *p), void *p)
{
    (*somefunction)(p);
}

/**
 * Submits work to the pool.
 */
int pool_submit(void (*somefunction)(void *p), void *p)
{
    task t;
    t.function = somefunction;
    t.data = p;
    enqueue(t);
    return 0;
}

// initialize the thread pool
void pool_init(void)
{
    rare=front=0;
    sem_init(&full,0,0);
    sem_init(&empty,0,QUEUE_SIZE);
    pthread_mutex_init(&mutex,NULL);
    for(int i=0;i<NUMBER_OF_THREADS;i++)
        pthread_create(&bee[i],NULL,worker,NULL);
}

// shutdown the thread pool
void pool_shutdown(void)
{
    for(int i=0;i<NUMBER_OF_THREADS;i++)
        pthread_cancel(bee[i]);
}
