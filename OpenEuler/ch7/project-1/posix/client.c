/**
 * Example client program that uses thread pool.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "threadpool.h"

struct data
{
    int a;
    int b;
};

void add(void *param)
{
    struct data *temp;
    temp = (struct data*)param;

    printf("I add two values %d and %d result = %d\n",temp->a, temp->b, temp->a + temp->b);
}

int main(void)
{
    // create some work to do
    struct data work[64];
    srand(42);
    for(int i=0;i<64;i++)
    {
        work[i].a = rand()%65536;
        work[i].b = rand()%65536;
    }

    // initialize the thread pool
    pool_init();

    // submit the work to the queue
    for(int i=0;i<64;i++)
        pool_submit(&add,&work[i]);

    // may be helpful 
    sleep(3);

    pool_shutdown();

    return 0;
}
