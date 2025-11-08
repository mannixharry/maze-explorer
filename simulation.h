#ifndef SIMULATION_H
#define SIMULATION_H

#include "robot_actions.h"
#include "grid.h"
#include "draw_grid.h"
#include "draw_robot.h"
#include "robot_memory.h"
#include "robot_pathfind.h"

void initialize_simulation(Robot *robot, RobotRender *robot_render, Grid *grid, GridView *grid_view, int tick_duration);
void sim_instructions(void);

bool investigate_ahead(RobotMemory *mem, RelativePosition curr_pos);


bool investigate_left(RobotMemory *mem, RelativePosition curr_pos);

bool investigate_right(RobotMemory *mem, RelativePosition curr_pos);
bool investigate_tile(RobotMemory *mem, RelativePosition curr_pos);

int check_spin(RobotMemory *mem, RelativePosition curr_pos);


#endif