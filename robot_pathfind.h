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

Direction* find_path_to_known(RobotMemory *mem, RelativePosition start, int *out_length);
RelativePosition get_pos_ahead(RelativePosition tile_pos, Direction direction);

#endif