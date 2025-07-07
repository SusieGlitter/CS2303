#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct Para
{
    int *a;
    int *b;
    int s;
    int t;
}para;

void para_set(para *p,int *a,int *b,int s,int t)
{
    p->a=a;p->b=b;p->s=s;p->t=t;
}

void *merge(void *pa)
{
    para *p=(para*)pa;
    int mid=(p->s+p->t)/2;
    int i=p->s,j=mid+1,k=p->s;
    while(i<=mid||j<=p->t)
        if(j==p->t+1||i<=mid&&p->a[i]<=p->a[j])
            p->b[k++]=p->a[i++];
        else
            p->b[k++]=p->a[j++];
    for(k=p->s;k<=p->t;k++)
        p->a[k]=p->b[k];
    pthread_exit(0);
}


void *mergesort(void *pa)
{
    para *p=(para*)pa;

    // printf("%d %d\n",p->s,p->t);

    if(p->s==p->t)
        pthread_exit(0);
    int mid=(p->s+p->t)/2;
    pthread_t tid1,tid2,tid3;
    pthread_attr_t tattr1,tattr2,tattr3;
    pthread_attr_init(&tattr1);
    pthread_attr_init(&tattr2);
    pthread_attr_init(&tattr3);
    para *p1=(para*)malloc(sizeof(para));
    para *p2=(para*)malloc(sizeof(para));
    para_set(p1,p->a,p->b,p->s,mid);
    para_set(p2,p->a,p->b,mid+1,p->t);
    pthread_create(&tid1,&tattr1,mergesort,p1);
    pthread_create(&tid2,&tattr2,mergesort,p2);
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    pthread_create(&tid3,&tattr3,merge,p);
    pthread_join(tid3,NULL);
    free(p);
    
    pthread_exit(0);
}

int a[64];
int b[64];
int n;

int main()
{
    scanf("%d",&n);
    for(int i=0;i<n;i++)
        scanf("%d",&a[i]);
    for(int i=0;i<n;i++)
        printf("%d ",a[i]);printf("\n");
    pthread_t tid;
    pthread_attr_t tattr;
    pthread_attr_init(&tattr);
    para *p=(para*)malloc(sizeof(para));
    para_set(p,a,b,0,n-1);
    pthread_create(&tid,&tattr,mergesort,p);
    pthread_join(tid,NULL);
    
    for(int i=0;i<n;i++)
        printf("%d ",a[i]);printf("\n");
    return 0;
}
