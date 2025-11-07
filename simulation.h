#ifndef SIMULATION_H
#define SIMULATION_H

#include "robot_actions.h"
#include "grid.h"
#include "draw_grid.h"
#include "draw_robot.h"
#include "robot_memory.h"


void initialize_simulation(Robot *robot, RobotRender *robot_render, Grid *grid, GridView *grid_view, int tick_duration);
void sim_instructions(void);



#endif