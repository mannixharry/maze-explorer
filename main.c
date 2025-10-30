#include "graphics.h"
#include <stdio.h>
#include <math.h>

const int screen_width = 600; 
const int screen_height = 600; 
const int wait_time = 25;
const int robot_size = 100;
const int robot_shape = 3; /* Number of sides */

typedef struct Coord
{
    double x;
    double y; 
} Coord; 

const Coord screen_centre = {screen_width/2, screen_height/2};

void calculate_regular_polygon_coords(int number_of_sides, Coord* polygon_coords, double angle) /* Array decays to a pointer */
{
    for(int i=0; i<number_of_sides; i++)
    {
        double x_i = cos(angle + 2 * M_PI * i / number_of_sides);
        double y_i = sin(angle + 2 * M_PI * i / number_of_sides);
        Coord coord_i = {x_i, y_i};
        polygon_coords[i] = coord_i;
    }
}

void transform_coordinates_to_screen(int size, Coord* coords, Coord screen_centre, double scale_factor)
{
    for (int i = 0; i < size; i++)
    {
        double current_x = coords[i].x;
        double current_y = coords[i].y;

        double transformed_x = screen_centre.x + current_x * scale_factor;
        double transformed_y = screen_centre.y - current_y * scale_factor; /* Subtraction so that coordinates work like Cartesian*/

        coords[i].x = (int)transformed_x;
        coords[i].y = (int)transformed_y; 
    }
}

void split_coords(int size, Coord* coords, int* xs, int* ys)
{
    for (int i = 0; i < size; i++)
    {
        xs[i] = coords[i].x;
        ys[i] = coords[i].y;
    }
} 

void draw_robot(double angle)
{
    const int s = 3; 
    Coord triangle_coords[s];
    calculate_regular_polygon_coords(s, triangle_coords, angle);
    transform_coordinates_to_screen(s, triangle_coords, screen_centre, robot_size);        
    
    int xs[s];
    int ys[s];
    split_coords(s, triangle_coords, xs, ys);

    clear();
    setColour(blue);
    fillPolygon(s, xs, ys);
}

int main(void)
{
    setWindowSize(screen_width, screen_height);
    double period = 10;
    double frames_per_second = 120;
    double tick = 1000/frames_per_second;

    double angle = 0;
    while (1) 
    {
        draw_robot(angle);
        angle += (2*M_PI / (period * frames_per_second) );
        sleep(tick);
        
    }

}

