#include "levels.h"

static int random_range(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

void set_markers_L1(Grid *grid)
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

    set_tile(grid, (TilePosition){r,c}, MARKER);
}

void set_obstacles_L1(Grid *grid)
{
    return;
}



void set_markers_L2(Grid *grid)
{

};
void set_markers_L3(Grid *grid) {

};
void set_markers_L4(Grid *grid) {

};
void set_markers_L5(Grid *grid) {

};


void set_obstacles_L2(Grid *grid) {
    
};
void set_obstacles_L3(Grid *grid) {

};
void set_obstacles_L4(Grid *grid) {
    
};
void set_obstacles_L5(Grid *grid) {

};