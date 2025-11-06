#ifndef ROBOT_ACTIONS_H
#define ROBOT_ACTIONS_H

#include "types.h"
#include "draw_grid.h"
#include "grid.h"

bool robot_update_animation(RobotRender *robot);
bool robot_can_move_forward(Robot *robot, Grid *grid);
void robot_forward(Robot *robot, Grid *grid, const GridView *grid_view);
void robot_left(Robot *robot);
void robot_right(Robot *robot);
bool robot_at_marker(Robot *robot, const Grid *grid);
void robot_pick_up_marker(Robot *robot, Grid *grid, const GridView *grid_view);
void robot_drop_marker(Robot *robot, Grid *grid, const GridView *grid_view);

#endif