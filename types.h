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

typedef enum Tile {EMPTY, MARKER, OBSTACLE} Tile;

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
    Coord start_position;
    Coord end_position; 
    Direction start_direction;
    Direction end_direction; 
    int number_of_frames;
    int current_frame;
} Animation;

typedef struct RobotRender
{
    Coord screen_position; 
    double angle; 
    int size;
    Animation animation;  
} RobotRender;

typedef struct Robot
{
    RobotRender robot_render; 
    Direction direction; 
    TilePosition grid_position;
    int marker_count;

} Robot; 


#endif