#include "buffer.h"

buffer_item buffer[BUFFER_SIZE];
int rare,front;
sem_t empty,full;
pthread_mutex_t mutex;

void buffer_init()
{
    rare=front=0;
    sem_init(&empty,0,BUFFER_SIZE);
    sem_init(&full,0,0);
    pthread_mutex_init(&mutex,NULL);
}
int insert_item(buffer_item item)
{
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);
    buffer[rare++]=item;
    rare%=BUFFER_SIZE;
    pthread_mutex_unlock(&mutex);
    sem_post(&full);
    
    return 0;
}
int remove_item(buffer_item *item)
{
    sem_wait(&full);
    pthread_mutex_lock(&mutex);
    *item=buffer[front++];
    front%=BUFFER_SIZE;
    pthread_mutex_unlock(&mutex);
    sem_post(&empty);
    
    return 0;
}
