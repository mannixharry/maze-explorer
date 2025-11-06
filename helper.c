#include "helper.h"

TilePosQueue create_tile_pos_queue(size_t max_size)
{
    TilePosition *tiles = malloc(max_size * sizeof(TilePosition));
    int head = 0;
    int tail = 0;

    return (TilePosQueue){head, tail, max_size, tiles};
}

bool is_tile_queue_empty(const TilePosQueue* queue)
{
    return (queue->head == queue->tail);
}

void enqueue_tile(TilePosQueue* queue, TilePosition tile_pos)
{
    if (queue->tail >= queue->max_size)
    {
        fprintf(stderr, "Queue overflow error");
        exit(1);
    }
    queue->tiles[queue->tail++] = tile_pos;
}

TilePosition dequeue_tile(TilePosQueue* queue)
{
    if (is_tile_queue_empty(queue)) 
    {
        fprintf(stderr, "Queue underflow error");
        exit(1);
    }
    return queue->tiles[queue->head++];
}

void free_tile_queue(TilePosQueue* queue)
{
    free(queue->tiles);
    queue->tiles = NULL;
}