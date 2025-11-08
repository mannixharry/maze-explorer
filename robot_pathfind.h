#ifndef ROBOT_PATHFIND_H
#define ROBOT_PATHFIND_H

#include "robot_memory.h"
#include "queue.h"

typedef struct Node
{
    RelativePosition pos;
    struct Node *parent;
    Direction par_dir;
    int depth;
} Node;

Direction* find_path_in_memory(RobotMemory *mem, RelativePosition start, int *out_length);
RelativePosition get_pos_ahead(RelativePosition tile_pos, Direction direction);
void write_path_to_file(const Direction *dir, int path_length, const char *filename);

#endif