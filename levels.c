#include "levels.h"

static int random_range(int a, int b);
static int min(int a, int b);
static int square(int x);

static TilePosition get_random_tile(const Grid *grid);
static TilePosition get_random_wall_adj_tile(const Grid *grid);
static Direction random_direction(void);
static int random_step_size(Grid *grid);
static int random_cluster_size(Grid *grid);

static bool try_place_obstacle(Grid *grid, TilePosition pos);
static int set_obstacles_in_line(Grid *grid, TilePosition *curr, Direction dir, int steps);
static int set_obstacle_cluster(Grid *grid, TilePosition cluster_pos);
static int set_obstacle_clusters(Grid *grid, double fill_fraction);
static void set_obstacle_circle(Grid *grid, int radius);

static void copy_grid(Grid *to_grid, Grid *from_grid);
static void fill_unreachable_space(Grid *grid, TilePosition start_pos);

static void set_random_wall_adj_marker(Grid *grid);
static void set_random_marker(Grid *grid);
static void set_random_markers(Grid *grid, int marker_count);

static bool in_circle(TilePosition pos, int radius, double centre_x, double centre_y);

void set_markers_L1(Grid *grid)
{
    set_random_wall_adj_marker(grid);
}

void set_markers_L2(Grid *grid)
{
    set_random_wall_adj_marker(grid);
}

void set_markers_L3(Grid *grid) 
{
    TilePosition random_tile = get_random_tile(grid);
    while (get_tile(grid, random_tile) != EMPTY)
    {
        TilePosition random_wall_adjacent_tile = get_random_wall_adj_tile(grid);
    }
    set_tile(grid, random_tile, MARKER);
}

void set_markers_L4(Grid *grid)
{
    const int min_markers_L4 = 2;
    const int max_markers_L4 = 5;
    int marker_count = random_range(min_markers_L4, max_markers_L4);

    set_random_markers(grid, marker_count);
}

void set_markers_L5(Grid *grid) 
{
    const int min_markers_L5 = 2;
    const int max_markers_L5 = 5;
    int marker_count = random_range(min_markers_L5, max_markers_L5);

    set_random_markers(grid, marker_count);
}

void set_markers_LS(Grid *grid) {}


void set_obstacles_L1(Grid *grid) {}

void set_obstacles_L2(Grid *grid) {}

void set_obstacles_L3(Grid *grid) {}

void set_obstacles_L4(Grid *grid)
{
    const double fill_fraction_L4 = 1.0 / 3.0;
    set_obstacle_clusters(grid, fill_fraction_L4);
    TilePosition grid_centre = {grid->rows / 2, grid->columns / 2};
    set_tile(grid, grid_centre, EMPTY);
    fill_unreachable_space(grid, grid_centre);
}

void set_obstacles_L5(Grid *grid) 
{
    const double circle_grid_ratio = 0.7;
    const int L5_circle_radius = round(0.5 * min(grid->rows, grid->columns) * circle_grid_ratio);
    const double fill_fraction_L5 = 0.2;

    set_obstacle_clusters(grid, fill_fraction_L5);
    TilePosition grid_centre = {grid->rows / 2, grid->columns / 2};
    set_tile(grid, grid_centre, EMPTY);
    set_obstacle_circle(grid, L5_circle_radius);
    fill_unreachable_space(grid, grid_centre);
}

void set_obstacles_LS(Grid *grid) 
{
    fill_grid(grid, OBSTACLE);
}

static int random_range(int a, int b)
{
    return a + rand() % (b - a + 1);
}

static int min(int a, int b)
{
    return a <= b ? a : b;
}

static int square(int x)
{
    return x * x;
}

static TilePosition get_random_tile(const Grid *grid)
{
    int r = rand() % grid->rows;
    int c = rand() % grid->columns;
    return (TilePosition){r, c};
}

static TilePosition get_random_wall_adj_tile(const Grid *grid)
{
    int r, c;
    int edge = rand() % 4;
    switch (edge)
    {
        case 0:
        r = 0; c = rand() % grid->columns; break;
        case 1:
        r = grid->rows - 1; c = rand() % grid->columns; break;
        case 2: 
        r = rand() % grid->rows; c = 0; break;
        case 3:
        r = rand() % grid->rows; c = grid->columns - 1; break;
    }
    return (TilePosition){r,c};
}

static Direction random_direction(void)
{
    return (Direction){rand() % 4};
}

static int random_step_size(Grid *grid)
{
    const int max_step_size = min(grid->rows, grid->columns)/5;
    return 1 + rand() % max_step_size;
}

static int random_cluster_size(Grid *grid)
{
    const int min_cluster_size = min(grid->rows, grid->columns)/5;
    return min_cluster_size + rand() % (int)(1.5 * min_cluster_size);
}

static bool try_place_obstacle(Grid *grid, TilePosition pos)
{
    if (get_tile(grid, pos) != EMPTY)
        return false;
    
    set_tile(grid, pos, OBSTACLE);
    return true;
}

static int set_obstacles_in_line(Grid *grid, TilePosition *curr, Direction dir, int steps) 
{
    int placed = 0;

    for (int i = 0; i < steps; i++)
        {
            if (try_place_obstacle(grid, *curr)) {
                placed++;
            }

            TilePosition next = get_tile_ahead(grid, *curr, dir);
            if (!check_tile_in_bounds(grid, next)) {return placed;}

            *curr = next;
        }
    return placed;
}

static int set_obstacle_cluster(Grid *grid, TilePosition cluster_pos)
{
    int placed = 0;
    TilePosition curr_pos = cluster_pos;
    int cluster_size = random_cluster_size(grid);
    while (placed < cluster_size)
    {
        Direction rand_dir = random_direction();
        int rand_step = random_step_size(grid);

        placed += set_obstacles_in_line(grid, &curr_pos, rand_dir, rand_step);
    }
    return placed;
}

static int set_obstacle_clusters(Grid *grid, double fill_fraction)
{
    int placed = 0; 
    const int min_obstacle_count = round(grid->rows * grid->columns * fill_fraction);
    while (placed < min_obstacle_count) 
    {
        TilePosition cluster_centre = get_random_tile(grid);
        placed += set_obstacle_cluster(grid, cluster_centre);
    }
}

static void set_obstacle_circle(Grid *grid, int radius)
{
    double centre_x = grid->rows / 2.0;
    double centre_y = grid->columns / 2.0;

    for (int r = 0; r < grid->rows; r++)
    {
        for (int c = 0; c < grid->columns; c++)
        {
            TilePosition pos = {r, c};
            if (!in_circle(pos, radius, centre_x, centre_y)) 
            {
                set_tile(grid, pos, OBSTACLE);
            }
        }
    }
}

static void copy_grid(Grid *to_grid, Grid *from_grid)
{
    if (to_grid->rows != from_grid->rows || to_grid->columns != from_grid->columns)
    {
        fprintf(stderr, "Grid dimensions do not match");
        return;
    }

    for (int r = 0; r < to_grid->rows; r++)
    {
        for (int c = 0; c < to_grid->columns; c++)
            {
                Tile type = get_tile(from_grid, (TilePosition){r,c});
                set_tile(to_grid, (TilePosition){r,c}, type);
            }
    }
}

static void fill_unreachable_space(Grid *grid, TilePosition start_pos)
{
    TilePosQueue tile_queue = create_tile_pos_queue((size_t){grid->rows * grid->columns});
    enqueue_tile(&tile_queue, start_pos);
    
    Grid *shadow_grid = create_grid(grid->rows, grid->columns, SHADOW);
    
    while(!is_tile_queue_empty(&tile_queue))
    {
        TilePosition curr_tile = dequeue_tile(&tile_queue);
        for (int i = 0; i < DIRECTION_COUNT; i++)
        {
            Direction dir = i;
            TilePosition tile_ahead = get_tile_ahead(grid, curr_tile, dir);

            bool in_bounds = check_tile_in_bounds(grid, tile_ahead);
            bool tile_ahead_empty = get_tile(grid, tile_ahead) == EMPTY;
            bool tile_unvisited = get_tile(shadow_grid, tile_ahead) == OBSTACLE;

            if (in_bounds && tile_ahead_empty && tile_unvisited)
            {
                set_tile(shadow_grid, tile_ahead, EMPTY);
                enqueue_tile(&tile_queue, tile_ahead);
            }
        }
    }
    copy_grid(grid, shadow_grid);
    free_grid(shadow_grid);
    free_tile_queue(&tile_queue);
}

static void set_random_wall_adj_marker(Grid *grid)
{
    TilePosition random_wall_adjacent_tile = get_random_wall_adj_tile(grid);
    while (get_tile(grid, random_wall_adjacent_tile) != EMPTY)
    {
        random_wall_adjacent_tile = get_random_wall_adj_tile(grid);
    }
    set_tile(grid, random_wall_adjacent_tile, MARKER);
}

static void set_random_marker(Grid *grid)
{
    TilePosition random_tile = get_random_tile(grid);
    while (get_tile(grid, random_tile) != EMPTY)
    {
        random_tile = get_random_tile(grid);
    }
    set_tile(grid, random_tile, MARKER);
}

static void set_random_markers(Grid *grid, int marker_count)
{
    for (int i = 0; i < marker_count; i++)
    {
        set_random_marker(grid);
    }
}

static bool in_circle(TilePosition pos, int radius, double centre_x, double centre_y)
{
    double pos_x = pos.row + 0.5;
    double pos_y = pos.column + 0.5;

    return square(pos_x - centre_x) + square(pos_y - centre_y) < square(radius); 
}