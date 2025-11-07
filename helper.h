#ifndef HELPER_H
#define HELPER_H

#include "types.h"

typedef struct TilePosQueue 
{
    int head, tail;
    size_t max_size;
    TilePosition *tiles; 

} TilePosQueue;

int min(int a, int b);
int max(int a, int b);
int random_range(int a, int b);
int square(int x);



TilePosQueue create_tile_pos_queue(size_t max_size);
bool is_tile_queue_empty(const TilePosQueue* queue);
void enqueue_tile(TilePosQueue* queue, TilePosition tile_pos);
TilePosition dequeue_tile(TilePosQueue* queue);
void free_tile_queue(TilePosQueue* queue);


#endif