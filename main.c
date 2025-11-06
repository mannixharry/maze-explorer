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

    background();

    Grid *grid = create_grid(20, 20, (Level){FIVE});

    GridView grid_view = {25, screen};
    draw_grid(grid, &grid_view);

    //draw_marker(&grid, &grid_view, (GridPosition){0,0});
    foreground();

    int starting_row = 3;
    int starting_column = 0;
    // Row then column

    Coord starting_screen_position = get_tile_coord(grid, &grid_view, (TilePosition){starting_row, starting_column}); 
    double angle = 0;

    Robot robot = {.robot_render = {starting_screen_position, angle, robot_size},
    .grid_position = {starting_row, starting_column},
    .direction = EAST, 
    .marker_count = 0};

    Coord test_pos = get_tile_coord(grid, &grid_view, (TilePosition){19, 19});
    forward(&robot, grid, &grid_view);

    while (1) 
    {
        if (update_animation(&robot.robot_render))
        {
            sleep(100);
            if (at_marker(&robot, grid))
            {
                pick_up_marker(&robot, grid, &grid_view);
                continue;
            }

            if (can_move_forward(&robot, grid))
            {
                forward(&robot, grid, &grid_view);
            }
            else 
            {
                drop_marker(&robot, grid, &grid_view);
                right(&robot);
            }
        }

        draw_robot(&robot.robot_render);
        sleep(tick);
    }

    }

/*
- Tidy up and optimize the code 
- Think about dynamic arrays etc 
- Get a nicer abstraction of the movement. 
- THEN WORRY ABT CODE TO CONTROL
*/


