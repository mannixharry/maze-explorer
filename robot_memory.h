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
    int row, column;
} RelativePosition;

typedef struct RobotMemory
{
    Tile **data; 
    Tile *flat_data;
    int rows, columns;
    MemoryPosition origin_pos;
} RobotMemory;

void write_memory_to_file(RobotMemory *mem, const char *filename);

RobotMemory* create_memory(int rows, int columns);
void set_tile_in_memory(RobotMemory *robot_memory, RelativePosition pos, Tile tile);
Tile get_tile_in_memory(RobotMemory *robot_memory, RelativePosition pos);
void free_memory(RobotMemory *robot_memory);

#endif
