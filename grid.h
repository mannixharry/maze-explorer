#ifndef GRID_H
#define GRID_H

#include "draw_grid.h"
#include "types.h"
#include "levels.h"

Grid* create_grid(int rows, int columns, Level level);
void free_grid(Grid *grid);

void set_tile(Grid *grid, TilePosition pos, Tile tile);
Tile get_tile(const Grid *grid, TilePosition pos);
TilePosition get_tile_ahead(const Grid *grid, TilePosition tile_pos, Direction direction);
bool check_tile_in_bounds(const Grid *grid, TilePosition tile_pos);
void fill_grid(Grid *grid, Tile tile);

#endif