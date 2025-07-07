#include "buffer.h"
typedef int buffer_item;
#define BUFFER_SIZE 5

void *producer(void *param)
{
    printf("\033[0;32m> producer created\n\033[0m");
    buffer_item item;
    
    while(1)
    {
        sleep(rand()%4+1);
        item=rand();
        if(insert_item(item))
            printf("\033[0;32m> producer report error condition\n\033[0m");
        else
            printf("\033[0;32m> producer produced %d\n\033[0m",item);
    }
}
void *consumer(void *param)
{
    printf("\033[0;31m< consumer created\n\033[0m");
    buffer_item item;
    
    while(1)
    {
        sleep(rand()%4+1);
        if(remove_item(&item))
            printf("\033[0;31m< consumer report error condition\n\033[0m");
        else
            printf("\033[0;31m< consumer consumed %d\n\033[0m",item);
    }
}
pthread_t p_tid[256];
pthread_t c_tid[256];

int main(int argc,char *argv[])
{
    int t=strtol(argv[1],NULL,10);
    int p=strtol(argv[2],NULL,10);
    int c=strtol(argv[3],NULL,10);
    
    buffer_init();
    
    for(int i=0;i<p;i++)
        pthread_create(&p_tid[i],NULL,producer,NULL);
    for(int i=0;i<c;i++)
        pthread_create(&c_tid[i],NULL,consumer,NULL);
        
    sleep(t);
    
    
    for(int i=0;i<p;i++)
        pthread_cancel(p_tid[i]);
    for(int i=0;i<c;i++)
        pthread_cancel(c_tid[i]);
    
    return 0;
}
