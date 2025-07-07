/**
 * "Virtual" CPU that also maintains track of system time.
 */

#include <stdio.h>

#include "task.h"

int t=0;

// run this task for the specified time slice
void run(Task *task, int slice) {
    printf("t=%d\tRunning task = [%s] [%d] [%d]\tfor %d units.\n",t,task->name, task->priority, task->burst, slice);
    t+=slice;
}
