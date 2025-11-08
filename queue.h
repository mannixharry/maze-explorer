#ifndef QUEUE_H
#define QUEUE_H

#include "types.h"
#include <string.h>
typedef struct Queue
{
    void* data; 
    int head, tail;
    size_t item_size;
    int capacity;
} Queue;


#include "queue.h"

Queue create_queue(int capacity, size_t item_size);
bool queue_empty(Queue *queue);
void enqueue_item(Queue *queue, void* item);
void dequeue_item(Queue *queue, void* out);
void free_queue(Queue *queue);








#endif