#include "simulation.h"

typedef struct SimulationContext
{
    Robot *robot;
    RobotRender *robot_render;
    Grid *grid;
    GridView *grid_view;
    int tick;
} SimulationContext;

static SimulationContext sim;


void initialize_simulation(Robot *robot, RobotRender *robot_render, Grid *grid, GridView *grid_view, int tick_duration)
{
    sim.robot = robot;
    sim.robot_render = robot_render;
    sim.grid = grid;
    sim.grid_view = grid_view;
    sim.tick = tick_duration;
}

static void complete_animation(void)
{
    bool anim_complete = robot_update_animation(sim.robot_render);
    while (!anim_complete) 
    {
        sleep(sim.tick);
        anim_complete = robot_update_animation(sim.robot_render);
        draw_robot(sim.robot_render);
    }
}

static bool can_move_forward(void)
{
    return robot_can_move_forward(sim.robot, sim.grid);
}

static void forward(void)
{
    robot_forward(sim.robot, sim.robot_render, sim.grid, sim.grid_view);
    complete_animation();
}

static void left(void)
{
    robot_left(sim.robot, sim.robot_render);
    complete_animation();
}

static void right(void)
{
    robot_right(sim.robot, sim.robot_render);
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
    sim.robot->marker_count = 100;
    RobotMemory *mem = create_memory(1, 1);
    RelativePosition curr_tile = {0,0};

    while(get_tile_in_memory(mem, curr_tile) != EMPTY)
    {
        while(can_move_forward())
        {   
            set_tile_in_memory(mem, curr_tile, EMPTY);
            curr_tile = get_pos_ahead(curr_tile, sim.robot->dir);
            draw_debug(sim.grid, sim.grid_view, sim.robot->grid_pos);
            forward();
            if (get_tile_in_memory(mem, curr_tile) == EMPTY) {break;}
        }
        while(!can_move_forward())
        {
            left();
        }
    }

    set_tile_in_memory(mem, curr_tile, EMPTY);

    for(int i= 0; i<4; i++)
    {
        left();
    }
    
    int path_length = 0;
    write_memory_to_file(mem, "robot_memory.txt");
    Direction *dir = find_path_in_memory(mem, curr_tile, (RelativePosition){0,0}, &path_length);
    write_path_to_file(dir, path_length, "path.txt");
    free_memory(mem);
    free(dir);

}