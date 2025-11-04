#include "grid.h"

Grid create_grid(int rows, int columns)
{
    // maybe I can dynamically allocate the grid here to avoid copying????
    Grid grid = create_empty_grid(rows, columns);
    PositionArray marker_positions = create_markers(&grid);
    PositionArray obstacle_positions = create_obstacles(&grid);

    add_cells_to_grid(&grid, marker_positions, MARKER);
    add_cells_to_grid(&grid, obstacle_positions, OBSTACLE);

    free_position_array(&marker_positions);
    free_position_array(&obstacle_positions);

    return grid; 
}

PositionArray create_markers(Grid *grid)
{
    const int number_of_markers = grid->rows;
    GridPosition *markers = malloc(number_of_markers * sizeof(GridPosition));
    for (int i = 0; i < number_of_markers; i++)
    {
        markers[i] = (GridPosition){i,i};
    }
    PositionArray position_array = {markers, number_of_markers};
    return position_array;
}

PositionArray create_obstacles(Grid *grid)
{
    const int number_of_obstacles = grid->rows;
    GridPosition *obstacles = malloc(number_of_obstacles * sizeof(GridPosition));
    for (int i = 0; i < number_of_obstacles; i++)
    {
        obstacles[i] = (GridPosition){grid->rows-i-1,i};
    }
    PositionArray position_array = {obstacles, number_of_obstacles};
    return position_array;
}

Grid create_empty_grid(int rows, int columns)
{
    Cell** grid_layout = malloc(rows * sizeof(Cell *));
    for (int r = 0; r < rows; r++)
    {
        grid_layout[r] = malloc(columns * sizeof(Cell));
    }

    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < columns; c++)
        {
            grid_layout[r][c] = EMPTY;
        }
    }

    Grid grid = {rows, columns, grid_layout};
    return grid;
}

void add_cells_to_grid(Grid *grid, PositionArray position_array, Cell cell_type)
{

    for (int i = 0; i < position_array.positions_size; i++)
    {
        add_cell_to_grid(grid, position_array.positions[i], cell_type);
    }
}

void add_cell_to_grid(Grid *grid, GridPosition position, Cell cell_type)
{
    int row = position.row;
    int column = position.column;   

    if (row < 0 || row >= grid->rows || column < 0 || column >= grid->columns)
    {
        return;
    }

    if (grid->grid_layout[row][column] == EMPTY)
    {
        grid->grid_layout[row][column] = cell_type;
    }
}

void free_grid(Grid* grid)
{
    for (int r = 0; r < grid->rows; r++)
    {
        free(grid->grid_layout[r]);
    }
    free(grid->grid_layout);
}

void free_position_array(PositionArray *position_array)
{
    free(position_array->positions);
}