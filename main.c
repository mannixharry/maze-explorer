#include "main.h"

#include <stdio.h>
#include <math.h>

const Dimensions screen = {600, 600};
const int wait_time = 25;
const int robot_size = 8;

int main(void)
{
    srand(time(NULL));
    setWindowSize(screen.width, screen.height);

    double period = 10;
    double frames_per_second = 120;
    double tick = 1000/frames_per_second;

    Grid *grid = create_grid(20, 20, (Level){FIVE});

    GridView grid_view = {25, screen};
    draw_grid(grid, &grid_view);

    int starting_row = 10;
    int starting_column = 10;
    Coord starting_screen_position = get_tile_coord(grid, &grid_view, (TilePosition){starting_row, starting_column}); 
    double angle = 0;
    Robot robot = {.robot_render = {starting_screen_position, angle, robot_size},
    .grid_pos = {starting_row, starting_column},
    .dir = EAST, 
    .marker_count = 0};

    initialize_simulation(&robot, grid, &grid_view, tick);

    sim_instructions();
}

/*
- Tidy up and optimize the code 
- Think about dynamic arrays etc 
- Get a nicer abstraction of the movement. 
- THEN WORRY ABT CODE TO CONTROL
*/


