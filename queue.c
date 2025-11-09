#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

static void shift_queue(Queue *queue);
static void extend_queue(Queue *queue);

// Simple linear extensible queue.

Queue create_queue(int capacity, size_t item_size)
{
    void* data = malloc(capacity * item_size);
    if (!data) { fprintf(stderr, "Failed to malloc queue data\n"); exit(1); }
    int head = 0;
    int tail = 0;
    return (Queue){data, head, tail, item_size, capacity};
}

bool queue_empty(Queue *queue)
{
    return queue->head == queue->tail;
}

void enqueue_item(Queue *queue, void* item)
{
    if (queue->tail >= queue->capacity)
    {
        // Shifting if possible before extending helps to avoid fragmenting the heap (though this really isn't a concern.)
        if (queue->head != 0) {shift_queue(queue);}
        else {extend_queue(queue);}
    }
    // (char* cast because void* arithmetic is technically undefined.)
    void *dest = (char*)queue->data + queue->tail * queue->item_size;
    memcpy(dest, item, queue->item_size);
    queue->tail++;
}

void dequeue_item(Queue *queue, void* out)
{
    if(queue_empty(queue))
    {
        fprintf(stderr, "Queue underflow error\n");
        exit(1);
    }
    void *src = (char*)queue->data + queue->head * queue->item_size;
    memcpy(out, src, queue->item_size);
    queue->head++;
}

void free_queue(Queue *queue)
{
    free(queue->data);
    queue->data = NULL;
    queue->head = queue->tail = queue->capacity = 0;
}

static void shift_queue(Queue *queue)
{
    // Shift the queue's head back to the start of allocated memory.
    if (queue->head == 0) return;
    int size = queue->tail - queue->head;

    void *dest = queue->data;
    void *src = (char*)queue->data + queue->head * queue->item_size;

    memmove(dest, src, size * queue->item_size); //memmove because memcpy is undefined for overlapping data.

    queue->head = 0;
    queue->tail = size;
}

static void extend_queue(Queue *queue)
{
    queue->capacity *= 2;
    void* new_data = realloc(queue->data, queue->capacity * queue->item_size);
    if (!new_data) { fprintf(stderr, "Queue realloc extension failed\n"); exit(1); }
    queue->data = new_data;
}