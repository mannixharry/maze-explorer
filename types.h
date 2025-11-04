#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>
#include <math.h>
#include <stdlib.h>

typedef struct Screen
{
    int width;
    int height;
} Screen;

typedef struct Coord
{
    double x;
    double y; 
} Coord; 

typedef struct GridPosition 
{
    int row; 
    int column;
} GridPosition;

typedef enum Direction {NORTH, EAST, SOUTH, WEST} Direction;

typedef enum Cell {EMPTY, MARKER, OBSTACLE} Cell;

static const double direction_angles[] = {
    [NORTH] = -M_PI/2,
    [EAST] = 0.0,
    [SOUTH] = M_PI/2,
    [WEST] = M_PI
};

typedef struct GridView
{
    int cell_size;
    Screen screen; 
} GridView; 

typedef struct Grid 
{
    int rows, columns; 
    Cell** grid_layout;
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
    GridPosition grid_position;
    int marker_count;

} Robot; 


#endif