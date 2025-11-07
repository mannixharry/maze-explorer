#ifndef TYPES_H
#define TYPES_H

#define M_PI 3.14159265358979323846

#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct Dimensions
{
    int width, height;
} Dimensions;

typedef struct Coord
{
    int x, y;
} Coord; 

typedef struct TilePosition 
{
    int row; 
    int column;
} TilePosition;

typedef enum Direction {NORTH, EAST, SOUTH, WEST, DIRECTION_COUNT} Direction;

typedef enum Tile {EMPTY, MARKER, OBSTACLE, UNKNOWN} Tile;

typedef enum Level {ONE, TWO, THREE, FOUR, FIVE, SHADOW} Level;

static const double direction_angles[] = {
    [NORTH] = -M_PI/2,
    [EAST] = 0.0,
    [SOUTH] = M_PI/2,
    [WEST] = M_PI
};

typedef struct GridView
{
    int tile_size;
    Dimensions screen; 
} GridView; 

typedef struct Grid 
{
    int rows, columns; 
    Tile** grid_layout;
} Grid;

typedef struct Animation 
{
    Coord start_pos;
    Coord end_pos; 
    Direction start_dir;
    Direction end_dir; 
    int frame_count;
    int curr_frame;
} Animation;

typedef struct RobotRender
{
    Coord screen_pos; 
    double angle; 
    int size;
    Animation anim;  
} RobotRender;

typedef struct Robot
{
    TilePosition grid_pos;
    Direction dir; 
    int marker_count;
} Robot; 
#endif