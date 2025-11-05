#ifndef GRID_H
#define GRID_H

#include "draw_grid.h"
#include "types.h"
#include "levels.h"

Grid* create_grid(int rows, int columns, Level level);
void free_grid(Grid *grid);

void set_tile(Grid *grid, TilePosition pos, Tile tile);
Tile get_tile(const Grid *grid, TilePosition pos);

#endif