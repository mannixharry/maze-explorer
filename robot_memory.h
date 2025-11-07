#ifndef ROBOT_MEMORY_H
#define ROBOT_MEMORY_H

#include "types.h"
#include "helper.h"

typedef struct MemoryPosition 
{
    int row, column; 
} MemoryPosition;

typedef struct RelativePosition
{
    int x, y;
} RelativePosition;

typedef struct RobotMemory
{
    Tile **data; 
    Tile *flat_data;
    int rows, columns;
    MemoryPosition origin_pos;
} RobotMemory;

RobotMemory* create_memory(int rows, int columns);
void set_tile_in_memory(RobotMemory *robot_memory, RelativePosition pos, Tile tile);
Tile get_tile_in_memory(RobotMemory *robot_memory, RelativePosition pos);
void free_memory(RobotMemory *robot_memory);

#endif
