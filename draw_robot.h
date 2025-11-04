#ifndef DRAW_ROBOT_H
#define DRAW_ROBOT_H

#include "graphics_link.h"
#include "types.h"
#include <math.h>

extern const int robot_size;
extern const int ROBOT_SIDES;

extern const Coord RobotShape[];

void set_robot_shape(Coord* robot_polar_coords);
void set_robot_angle(Coord* robot_polar_coords, double angle);
void enlarge_robot_coords(Coord* robot_coords, double scale_factor);
void set_robot_position(Coord* robot_coords, Coord position);
void split_coords(Coord* coords, int number_of_coords, int* xs, int* ys);
void draw_robot(RobotRender*);
void draw_robot_fill(int*, int*);
void draw_robot_outline(int*, int*);
#endif