#include "grid.h"

static void (*marker_setters[])(Grid*) = {
    set_markers_L1,
    set_markers_L2,
    set_markers_L3,
    set_markers_L4,
    set_markers_L5,
    set_markers_LS,
};

static void (*obstacle_setters[])(Grid*) = {
    set_obstacles_L1,
    set_obstacles_L2,
    set_obstacles_L3,
    set_obstacles_L4,
    set_obstacles_L5,
    set_obstacles_LS,
};

static Grid* create_empty_grid(size_t rows, size_t columns);
static Grid* malloc_empty_grid(size_t rows, size_t columns);
static void set_markers(Grid *grid, Level level);
static void set_obstacles(Grid *grid, Level level);

Grid* create_grid(int rows, int columns, Level level)
{
    Grid *grid = create_empty_grid(
        (size_t)rows, (size_t)columns
    );
    
    if (ONE <= level && level <= SHADOW)
    {
        set_obstacles(grid, level); // very important obstacles set first
        set_markers(grid, level);
    }
    return grid; 
}

void free_grid(Grid* grid)
{
    for (int r = 0; r < grid->rows; r++)
    {
        free(grid->grid_layout[r]);
    }
    free(grid->grid_layout);
    free(grid);
}

void set_tile(Grid *grid, TilePosition pos, Tile tile)
{
    if (pos.row < 0 || pos.row >= grid->rows || pos.column < 0 || pos.column >= grid->columns) {return;}

    grid->grid_layout[pos.row][pos.column] = tile;
}

Tile get_tile(const Grid *grid, TilePosition pos) 
{
    if (pos.row < 0 || pos.row >= grid->rows || pos.column < 0 || pos.column >= grid->columns) {return OBSTACLE;}
    return grid->grid_layout[pos.row][pos.column];
}

TilePosition get_tile_ahead(const Grid *grid, TilePosition tile_pos, Direction direction)
{
    int row = tile_pos.row;
    int column = tile_pos.column;
    switch (direction)
    {
        case NORTH: row--; break;
        case EAST: column++; break;
        case SOUTH: row++; break;
        case WEST: column--; break;
    }
    return (TilePosition){row, column};
}

bool check_tile_in_bounds(const Grid *grid, TilePosition tile_pos)
{
    int r = tile_pos.row;
    int c = tile_pos.column;

    return (r >= 0 && r < grid->rows && c >= 0 && c < grid->columns);
}

void fill_grid(Grid* grid, Tile tile)
{
    for (size_t r = 0; r < grid->rows; r++)
    {
        for (size_t c = 0; c < grid->columns; c++)
        {
            set_tile(grid, (TilePosition){r,c}, tile);
        }
    }
}

static Grid* create_empty_grid(size_t rows, size_t columns)
{
    Grid *grid = malloc_empty_grid(rows, columns);
    if (!grid)
    {
        fprintf(stderr, "Failed to allocate memory for grid\n");
        exit(1);
    }

    grid->rows = rows;
    grid->columns = columns;
    
    fill_grid(grid, EMPTY);
    return grid;
}

static Grid* malloc_empty_grid(size_t rows, size_t columns)
{
    Grid *grid = malloc(sizeof(Grid));
    if (!grid) {return NULL;}

    Tile** grid_layout = malloc(rows * sizeof(Tile *));
    if (!grid_layout) 
    {
        free(grid);
        return NULL;
    }
    for (size_t r = 0; r < rows; r++)
    {
        grid_layout[r] = malloc(columns * sizeof(Tile));
        if (!grid_layout[r])
        {
            for(size_t i = 0; i < r; i++) 
            {
                free(grid_layout[i]);
            }
            free(grid_layout);
            free(grid);
            return NULL;
        }
    }
    grid->grid_layout = grid_layout;
    return grid;
}

static void set_markers(Grid *grid, Level level)
{
    marker_setters[level](grid);
}

static void set_obstacles(Grid *grid, Level level)
{
    obstacle_setters[level](grid);
}