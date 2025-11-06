#include "simulation.h"

typedef struct SimulationContext
{
    Robot *robot;
    Grid *grid;
    GridView *grid_view;
    int tick;
} SimulationContext;

static SimulationContext sim;
static int call_tracker = 0;

void initialize_simulation(Robot *robot, Grid *grid, GridView *grid_view, int tick_duration)
{
    sim.robot = robot;
    sim.grid = grid;
    sim.grid_view = grid_view;
    sim.tick = tick_duration;
}

static void complete_animation(void)
{
    bool anim_complete = robot_update_animation(&sim.robot->robot_render);
    while (!anim_complete) 
    {
        sleep(sim.tick);
        anim_complete = robot_update_animation(&sim.robot->robot_render);
        draw_robot(&sim.robot->robot_render);
    }
}

static bool can_move_forward(void)
{
    return robot_can_move_forward(sim.robot, sim.grid);
}

static void forward(void)
{
    robot_forward(sim.robot, sim.grid, sim.grid_view);
    complete_animation();
}

static void left(void)
{
    robot_left(sim.robot);
    complete_animation();
}

static void right(void)
{
    robot_right(sim.robot);
    complete_animation();
}

static bool at_marker(void)
{
    return robot_at_marker(sim.robot, sim.grid);
}

static void pick_up_marker(void)
{
    robot_pick_up_marker(sim.robot, sim.grid, sim.grid_view);
}

static void drop_marker(void)
{
    robot_drop_marker(sim.robot, sim.grid, sim.grid_view);
}

void sim_instructions(void)
{
    while(1)
    {
        while(can_move_forward())
        {
            forward();
        }
        while(!can_move_forward())
        {
            left();
        }
    }
}
