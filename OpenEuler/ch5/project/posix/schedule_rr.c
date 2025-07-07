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
    while(now->next!=NULL)
        now=now->next;
    int t=(now->task->burst>QUANTUM)?QUANTUM:now->task->burst;
    run(now->task,t);
    delete(&head,now->task);
    now->task->burst-=t;
    if(now->task->burst>0)
        insert(&head,now->task);
    else
    {
        free(now->task->name);
        free(now->task);
    }
}


void schedule()
{
    printf("Scheduling algorithm: rr\n");
    while(head!=NULL)
        pickNextTask();
}








