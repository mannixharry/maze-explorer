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
    draw_debug(sim.grid, sim.grid_view, sim.robot->grid_pos);
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

void align_cardinal_dir(Direction dir)
{
    int mod_diff = (dir - sim.robot->dir + DIRECTION_COUNT) % DIRECTION_COUNT;
    switch(mod_diff)
    {
        case 1: right(); break;
        case 2: left(); left(); break;
        case 3: left(); break;
    }
}

void follow_path(RobotMemory *mem, RelativePosition *curr_pos, Direction *dir, int path_length)
{
    for (int i = 0; i < path_length; i++)
    {
        align_cardinal_dir(dir[i]);
        *curr_pos = get_pos_ahead(*curr_pos, sim.robot->dir);
        set_tile_in_memory(mem, *curr_pos, EMPTY);
        forward();
    }
}

void view_surroundings(RobotMemory *mem, RelativePosition curr_tile)
{
    set_tile_in_memory(mem, curr_tile, EMPTY);
    left();
    if (!can_move_forward()) {set_tile_in_memory(mem, get_pos_ahead(curr_tile, sim.robot->dir), OBSTACLE);}
    right();
    if (!can_move_forward()) {set_tile_in_memory(mem, get_pos_ahead(curr_tile, sim.robot->dir), OBSTACLE);}
    right();
    if (!can_move_forward()) {set_tile_in_memory(mem, get_pos_ahead(curr_tile, sim.robot->dir), OBSTACLE);} 
    left();
}

void sim_instructions(void)
{
    sim.robot->marker_count = 100;
    RobotMemory *mem = create_memory(1, 1);
    RelativePosition curr_pos = {0,0};
    view_surroundings(mem, curr_pos);

    int path_length = 0;
    while (true)
    {
        Direction *dir = find_path_in_memory(mem, curr_pos, &path_length);
        if (!dir) {break;}

        follow_path(mem, &curr_pos, dir, path_length);
        view_surroundings(mem, curr_pos);
        free(dir);
        write_memory_to_file(mem, "robot_memory.txt");
    }
    free_memory(mem);

}
/*



    Direction *dir = find_path_in_memory(mem, curr_tile, &path_length);
    while()
    while(get_tile_in_memory(mem, curr_tile) != EMPTY)
    {
        while(can_move_forward())
        {   
            set_tile_in_memory(mem, curr_tile, EMPTY);
            curr_tile = get_pos_ahead(curr_tile, sim.robot->dir);
            //draw_debug(sim.grid, sim.grid_view, sim.robot->grid_pos);
            forward();
            if (get_tile_in_memory(mem, curr_tile) == EMPTY) {break;}
        }
        while(!can_move_forward())
        {
            left();
        }
    }

    int path_length = 0;
    write_memory_to_file(mem, "robot_memory.txt");
    Direction *dir = find_path_in_memory(mem, curr_tile, &path_length);
    write_path_to_file(dir, path_length, "path.txt");


    */