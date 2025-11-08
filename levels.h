#ifndef LEVELS_H
#define LEVELS_H

#include "types.h"
#include "grid.h"
#include "helper.h"
#include "queue.h"

TilePosition get_random_tile(const Grid *grid);
TilePosition get_random_empty_tile(const Grid *grid);
Direction get_random_direction(void);

void set_markers_L1(Grid *grid);
void set_markers_L2(Grid *grid);
void set_markers_L3(Grid *grid);
void set_markers_L4(Grid *grid);
void set_markers_L5(Grid *grid);
void set_markers_LS(Grid *grid);

void set_obstacles_L1(Grid *grid);
void set_obstacles_L2(Grid *grid);
void set_obstacles_L3(Grid *grid);
void set_obstacles_L4(Grid *grid);
void set_obstacles_L5(Grid *grid);
void set_obstacles_LS(Grid *grid);

#endif