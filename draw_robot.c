#include "draw_robot.h"
#include <math.h>

const int ROBOT_SIDES = 3; 

const Coord RobotShape[ROBOT_SIDES] = {
    {1, 0},  // ( 0, 1 )
    {-0.5, 0.8660254038}, // ( -0.5, sqrt(3)/2 )
    {-0.5, -0.8660254038} // ( -0.5, -sqrt(3)/2 )
};

void set_robot_shape(Coord* robot_coords)
{
    for (int i = 0; i < ROBOT_SIDES; i++)
    {
        robot_coords[i] = RobotShape[i];
    }
}

void set_robot_angle(Coord* robot_coords, double angle) 
{
    double sin_theta = sin(angle);
    double cos_theta = cos(angle);

    for (int i = 0; i < ROBOT_SIDES; i++)
    {
        double current_x = robot_coords[i].x;
        double current_y = robot_coords[i].y;
        
        double new_x = current_x * cos_theta - current_y * sin_theta;
        double new_y = current_x * sin_theta + current_y * cos_theta;

        robot_coords[i] = (Coord){new_x, new_y};
    }
}

void enlarge_robot_coords(Coord* robot_coords, double scale_factor)
{
    for (int i = 0; i < ROBOT_SIDES; i++)
    {
        double current_x = robot_coords[i].x;
        double current_y = robot_coords[i].y;

        double transformed_x = current_x * scale_factor;
        double transformed_y = current_y * scale_factor; /* Subtraction so that coordinates work like Cartesian*/

        robot_coords[i].x = transformed_x;
        robot_coords[i].y = transformed_y; 
    }
}

void set_robot_position(Coord* robot_coords, Coord position)
{
    for (int i=0; i < ROBOT_SIDES; i++)
    {
        robot_coords[i].x += position.x;
        robot_coords[i].y += position.y;
    }
}

void split_coords(Coord* coords, int number_of_coords, int* xs, int* ys)
{
    for (int i = 0; i < number_of_coords; i++)
    {
        xs[i] = round(coords[i].x);
        ys[i] = round(coords[i].y);
    }
} 

void draw_robot_fill(int *xs, int *ys)
{
    setColour(blue);
    fillPolygon(ROBOT_SIDES, xs, ys);
}

void draw_robot_outline(int *xs, int *ys)
{
    setLineWidth(3);
    setColour(blue);
    drawPolygon(ROBOT_SIDES, xs, ys);
    setColour(red);
    setLineWidth(3);
    drawLine(xs[1], ys[1], xs[2], ys[2]);
    setLineWidth(1);
}

void draw_robot(RobotRender *robot)
{ 
    Coord robot_coords[ROBOT_SIDES];
    set_robot_shape(robot_coords);
    set_robot_angle(robot_coords, robot->angle);
    enlarge_robot_coords(robot_coords, robot->size);
    set_robot_position(robot_coords, robot->screen_position);       

    int xs[ROBOT_SIDES];
    int ys[ROBOT_SIDES];
    split_coords(robot_coords, ROBOT_SIDES, xs, ys);

    clear();
    draw_robot_fill(xs, ys);
    draw_robot_outline(xs, ys);

}