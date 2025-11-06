#ifndef DRAW_GRID_H
#define DRAW_GRID_H

#include "types.h"
#include "graphics_link.h"
#include "grid.h"

Coord get_tile_coord(const Grid *grid, const GridView *grid_view, TilePosition grid_position);

void draw_marker(const Grid *grid, const GridView *grid_view, TilePosition marker_position);
void draw_obstacle(const Grid *grid, const GridView *grid_view, TilePosition obstacle_position);
void draw_empty(const Grid *grid, const GridView *grid_view, TilePosition marker_position);
void draw_grid(const Grid *grid, const GridView *grid_view);

#endif

