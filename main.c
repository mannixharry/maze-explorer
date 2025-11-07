#include "main.h"

const Dimensions SCREEN = {800, 800};
const int GRID_WIDTH = 30;
const int GRID_HEIGHT = 30;

const double GRID_SCREEN_RATIO = 0.85;

const double FPS = 120;
const double TICK = 1000/FPS;

int main(void)
{
    const int cell_size = floor(GRID_SCREEN_RATIO * (double)min(SCREEN.height, SCREEN.width) / (double)max(GRID_WIDTH, GRID_HEIGHT));
    const int robot_size = floor((double)cell_size / 3.0);

    srand(time(NULL));
    setWindowSize(SCREEN.width, SCREEN.height);

    Grid *grid = create_grid(GRID_WIDTH, GRID_HEIGHT, (Level){FIVE});
    GridView grid_view = {cell_size, SCREEN};

    TilePosition start_pos = get_random_empty_tile(grid);
    Direction star_dir = get_random_direction();

    Coord start_screen_pos = get_tile_coord(grid, &grid_view, start_pos); 
    double angle = direction_angles[star_dir];

    RobotRender robot_render = {start_screen_pos, angle, robot_size};
    Robot robot = {start_pos, star_dir, 0};
    
    draw_grid(grid, &grid_view);

    initialize_simulation(&robot, &robot_render, grid, &grid_view, TICK);
    draw_robot(&robot_render);
    sleep(1000);
    sim_instructions();
}


