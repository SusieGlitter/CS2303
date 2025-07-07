typedef int buffer_item;
#define BUFFER_SIZE 5
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>

void buffer_init();
int insert_item(buffer_item item);
int remove_item(buffer_item *item);
