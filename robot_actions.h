#ifndef ROBOT_ACTIONS_H
#define ROBOT_ACTIONS_H

#include "types.h"
#include "draw_grid.h"
#include "grid.h"

bool update_animation(RobotRender*);

void forward(Robot*, Grid*, GridView*);
void left(Robot*);
void right(Robot*);
bool at_marker(Robot* robot, Grid *grid);
bool can_move_forward(Robot* robot, Grid *grid);
void pick_up_marker(Robot* robot, Grid* grid, GridView *grid_view);
void drop_marker(Robot* robot, Grid* grid, GridView *grid_view);

#endif

