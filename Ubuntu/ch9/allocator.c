#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PAGE_SIZE 4096
#define MEMORY_SIZE 1048576

typedef struct SEG
{
    int s;
    int t;
    int used;
    char *name;
    struct SEG *prev;
    struct SEG *next;
    struct SEG *smaller;
    struct SEG *bigger;
}seg;

seg *head;
seg *tail;

void init()
{
    seg *temp=(seg*)malloc(sizeof(seg));
    head=(seg*)malloc(sizeof(seg));
    tail=(seg*)malloc(sizeof(seg));
    
    temp->s=0;
    temp->t=MEMORY_SIZE;
    temp->used=0;
    temp->name=NULL;
    temp->prev=head;
    temp->next=tail;
    temp->smaller=head;
    temp->bigger=tail;

    head->s=0;
    head->t=0;
    head->used=1;
    head->name=NULL;
    head->prev=NULL;
    head->next=temp;
    head->smaller=NULL;
    head->bigger=temp;
    
    tail->s=MEMORY_SIZE;
    tail->t=2*MEMORY_SIZE+1;
    tail->used=1;
    tail->name=NULL;
    tail->prev=temp;
    tail->next=NULL;
    tail->smaller=temp;
    tail->bigger=NULL;
}

void ins(seg *now)
{
    seg *temp=head;
    while(temp->bigger)
    {
        temp=temp->bigger;
        if(temp->t-temp->s>=now->t-now->s)
        {
            now->smaller=temp->smaller;
            now->bigger=temp;
            temp->smaller=now;
            now->smaller->bigger=now;
            return;
        }
    }
}
void del(seg *now)
{
    now->bigger->smaller=now->smaller;
    now->smaller->bigger=now->bigger;
}

void request_at(int size,char *name,seg *now)
{
    printf("Allocate Addresses [%d:%d] For Process %s\n",now->s,now->s+size-1,name);
    now->used=1;
    now->name=name;
    if(size==now->t-now->s)
        return;
    seg *temp=(seg*)malloc(sizeof(seg));
    temp->s=now->s+size;
    temp->t=now->t;
    temp->used=0;
    temp->name=NULL;
    temp->prev=now;
    temp->next=now->next;
    
    now->t=temp->s;
    now->next=temp;
    del(now);
    ins(temp);
    ins(now);
}

void release_at(seg *now)
{
    printf("Deallocate Addresses [%d:%d] For Process %s\n",now->s,now->t-1,now->name);
    free(now->name);
    now->used=0;
    now->name=NULL;
    seg *temp=now->prev;
    if(temp!=head&&temp->used==0)
    {
        now->s=temp->s;
        now->prev=temp->prev;
        now->prev->next=now;
        del(temp);
        free(temp);
    }
    temp=now->next;
    if(temp!=tail&&temp->used==0)
    {
        now->t=temp->t;
        now->next=temp->next;
        now->next->prev=now;
        del(temp);
        free(temp);
    }
    del(now);
    ins(now);
}

int request()
{
    char *name=(char*)malloc(sizeof(char)*128);
    int size;
    char *strategy=(char*)malloc(sizeof(char)*128);
    scanf("%s %d %s",name,&size,strategy);
    size=((size-1)/4096+1)*4096;
    seg *target=NULL;
    seg *temp;
    if(strategy[0]=='F')
    {
        temp=head;
        while(temp->next!=tail)
        {
            temp=temp->next;
            if(temp->used)
                continue;
            if(temp->t-temp->s>=size)
            {
                target=temp;
                break;
            }
        }
    }
    else if(strategy[0]=='B')
    {
        temp=head;
        while(temp->bigger!=tail)
        {
            temp=temp->bigger;
            if(temp->used)
                continue;
            if(temp->t-temp->s>=size)
            {
                target=temp;
                break;
            }
        }
    }
    else if(strategy[0]=='W')
    {
        temp=tail;
        while(temp->smaller!=head)
        {
            temp=temp->smaller;
            if(temp->used)
                continue;
            if(temp->t-temp->s>=size)
            {
                target=temp;
                break;
            }
        }
    }
    free(strategy);
    if(target)
    {
        request_at(size,name,target);
        return 0;
    }
    else
    {
        printf("Allocation failed\n");
        return -1;
    }
}
void release()
{
    char *name=(char*)malloc(sizeof(char)*128);
    scanf("%s",name);
    seg *temp=head;
    while(temp->next!=tail)
    {
        temp=temp->next;
        if(temp->used==0)
            continue;
        if(strcmp(name,temp->name)==0)
            release_at(temp);
    }
}
void show_info()
{
    seg *temp=head;
    while(temp->next!=tail)
    {
        temp=temp->next;
        if(temp->used)
            printf("Addresses [%d:%d] Process %s\n",temp->s,temp->t-1,temp->name);
        else
            printf("Addresses [%d:%d] Unused\n",temp->s,temp->t-1);
            
    }
    /*
    printf("\n");
    temp=head;
    while(temp->bigger!=tail)
    {
        temp=temp->bigger;
        if(temp->used)
            printf("Addresses [%d:%d] Process %s\n",temp->s,temp->t-1,temp->name);
        else
            printf("Addresses [%d:%d] Unused\n",temp->s,temp->t-1);
            
    }
    */
}


int main()
{
    init();
    show_info();
    char *s=(char*)malloc(sizeof(char)*128);
    while(1)
    {
        printf(">");
        scanf("%s",s);
        if(s[1]=='T')
            show_info();
        else if(s[1]=='Q')
            request();
        else if(s[1]=='L')
            release();
    }
    return 0;
}
