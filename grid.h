#ifndef GRID_H
#define GRID_H

#include "draw_grid.h"
#include "types.h"

typedef struct PositionArray
{
    GridPosition *positions;
    int positions_size; 
} PositionArray;

Grid create_empty_grid(int rows, int columns);
Grid create_grid(int rows, int columns);
void free_grid(Grid *grid);


PositionArray create_markers(Grid *grid);
PositionArray create_obstacles(Grid *grid);
void free_position_array(PositionArray *position_array);

void add_cells_to_grid(Grid *grid, PositionArray position_array, Cell cell_type);
void add_cell_to_grid(Grid *grid, GridPosition position, Cell cell_type);

#endif