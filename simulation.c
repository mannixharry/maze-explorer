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

static void complete_animation(void);
static bool can_move_forward(void);
static void forward(void);
static void left(void);
static void right(void);
static bool at_marker(void);
static void pick_up_marker(void);
static void drop_marker(void);
void align_cardinal_dir(Direction dir);
static Direction get_left_dir(Direction dir);
static Direction get_right_dir(Direction dir);
static void follow_path(RobotMemory *mem, RelativePosition *curr_pos, Direction *dir, int path_length);
static void investigate_surroundings(RobotMemory *mem, RelativePosition curr_pos);
static void full_spin_investigate(RobotMemory *mem, RelativePosition curr_pos);
static int count_obstacles_near(RobotMemory *mem, RelativePosition curr_pos);

void sim_instructions(void)
{
    RelativePosition corner_position;
    sim.robot->marker_count = 100;
    RobotMemory *mem = create_memory(1, 1);
    RelativePosition curr_pos = {0,0};
    set_MTile_in_memory(mem, curr_pos, VISITED);
    full_spin_investigate(mem, curr_pos);
    int max_obstacles_near = count_obstacles_near(mem, curr_pos);
    if(at_marker()) {pick_up_marker();}

    int path_length = 0;
    while (true)
    {
        Direction *path = find_path_to_known(mem, curr_pos, &path_length);
        if (!path) {break;}

        follow_path(mem, &curr_pos, path, path_length);
        investigate_surroundings(mem, curr_pos);
        int obstacles_near = count_obstacles_near(mem, curr_pos);
        if (max_obstacles_near <= obstacles_near) {max_obstacles_near = obstacles_near; corner_position = curr_pos;}
        free(path);
    }

    set_MTile_in_memory(mem, corner_position, KNOWN);
    Direction *path = find_path_to_known(mem, curr_pos, &path_length);
    if (path != NULL) {follow_path(mem, &curr_pos, path, path_length); free(path);}
    drop_marker();
    write_memory_to_file(mem, "robot_memory.txt");
    free_memory(mem);
}


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

static Direction get_left_dir(Direction dir)
{
    return (dir - 1 + DIRECTION_COUNT) % DIRECTION_COUNT;
}

static Direction get_right_dir(Direction dir)
{
    return (dir + 1) % DIRECTION_COUNT;
}

static void follow_path(RobotMemory *mem, RelativePosition *curr_pos, Direction *dir, int path_length)
{
    for (int i = 0; i < path_length; i++)
    {
        align_cardinal_dir(dir[i]);
        *curr_pos = get_pos_ahead(*curr_pos, sim.robot->dir);
        set_MTile_in_memory(mem, *curr_pos, VISITED);
        forward();
        if(at_marker()) {pick_up_marker();}
    }
}

static void investigate_surroundings(RobotMemory *mem, RelativePosition curr_pos)
{
    Direction dir = sim.robot->dir;
    RelativePosition left_pos = get_pos_ahead(curr_pos, get_left_dir(dir));
    RelativePosition right_pos = get_pos_ahead(curr_pos, get_right_dir(dir));
    RelativePosition ahead_pos = get_pos_ahead(curr_pos, dir);

    if (get_MTile_in_memory(mem, ahead_pos) == UNKNOWN)
    {
        if (can_move_forward()) {set_MTile_in_memory(mem, ahead_pos, KNOWN);}
        else {set_MTile_in_memory(mem, get_pos_ahead(curr_pos, dir), M_OBSTACLE);}
    }

    if (get_MTile_in_memory(mem, right_pos) == UNKNOWN)
    {
        right();
        if (can_move_forward()) {set_MTile_in_memory(mem, right_pos, KNOWN);}
        else {set_MTile_in_memory(mem, right_pos, M_OBSTACLE);}
        left();
    }

    if (get_MTile_in_memory(mem, left_pos) == UNKNOWN)
    {
        left();
        if (can_move_forward()) {set_MTile_in_memory(mem, left_pos, KNOWN);}
        else {set_MTile_in_memory(mem, left_pos, M_OBSTACLE);}
        right();
    }
}

static void full_spin_investigate(RobotMemory *mem, RelativePosition curr_pos)
{
    for (int i = 0; i < DIRECTION_COUNT; i++)
    {
        RelativePosition ahead_pos = get_pos_ahead(curr_pos, sim.robot->dir);
        if (get_MTile_in_memory(mem, ahead_pos) == UNKNOWN)
        {
            if (can_move_forward()) {set_MTile_in_memory(mem, ahead_pos, KNOWN);}
            else {set_MTile_in_memory(mem, ahead_pos, M_OBSTACLE);}
            
        }
        right();
    }
}

static int count_obstacles_near(RobotMemory *mem, RelativePosition curr_pos)
{
    Direction dir = NORTH;
    int obstacle_count = 0;
    for (int i = 0; i < DIRECTION_COUNT; i++)
    {
        RelativePosition ahead_pos = get_pos_ahead(curr_pos, dir);
        if (get_MTile_in_memory(mem, ahead_pos) == M_OBSTACLE) {obstacle_count++;}
        dir = get_right_dir(dir);
    }
    return obstacle_count;
}
