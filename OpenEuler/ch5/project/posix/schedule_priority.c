#include "schedulers.h"
#include "list.h"
#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>



struct node *head=NULL;

void add(char *name, int priority, int burst)
{
    Task *tsk=(Task*)malloc(sizeof(Task));
    tsk->burst=burst;
    tsk->name=(char*)malloc(sizeof(char)*128);
    strcpy(tsk->name,name);
    tsk->priority=priority;
    insert(&head,tsk);
}

void pickNextTask()
{
    struct node *now=head;
    struct node *exe=head;
    while(now->next!=NULL)
    {
        now=now->next;
        if(now->task->priority>=exe->task->priority)
            exe=now;
    }
    run(exe->task,exe->task->burst);
    delete(&head,exe->task);
    free(exe->task->name);
    free(exe->task);
}


void schedule()
{
    printf("Scheduling algorithm: priority\n");
    while(head!=NULL)
        pickNextTask();
}








