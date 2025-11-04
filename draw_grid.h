#ifndef DRAW_GRID_H
#define DRAW_GRID_H

#include "types.h"
#include "graphics_link.h"

void draw_grid(Grid *grid, GridView *grid_view);
void draw_grid_lines(Grid *grid, GridView *grid_view);
void draw_grid_border(Grid *grid, GridView *grid_view);
void draw_marker(Grid *grid, GridView *grid_view, GridPosition marker_position);
void draw_obstacle(Grid *grid, GridView *grid_view, GridPosition obstacle_position);
void draw_empty(Grid *grid, GridView *grid_view, GridPosition marker_position);
void fill_grid_cell(Grid *grid, GridView *grid_view, GridPosition grid_position, colour colour);
void draw_grid_cells(Grid *grid, GridView *grid_view);

Coord get_grid_cell_coordinate(Grid *grid, GridView *grid_view, GridPosition grid_position);
Coord get_top_left(Grid *grid, int cell_size, Screen screen);

#endif

