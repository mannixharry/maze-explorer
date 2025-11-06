#include "draw_robot.h"
#include <math.h>

#define ROBOT_SIDES 3

#define ROBOT_COLOUR blue
#define ROBOT_OUTLINE_COLOUR red

#define ROBOT_LINE_WIDTH 3

typedef struct fCoord
{
    double x, y;
} fCoord;

static const fCoord RobotShape[ROBOT_SIDES] = {
    {1, 0}, // ( 0, 1 )
    {-0.5, 0.8660254038}, // ( -0.5, sqrt(3)/2 )
    {-0.5, -0.8660254038} // ( -0.5, -sqrt(3)/2 )
};

static void set_shape(fCoord *robot_fCoords);
static void set_angle(fCoord* robot_fCoords, double angle);
static void enlarge_fCoords(fCoord* robot_fCoords, double scale_factor);
static void set_pos(fCoord* robot_fCoords, Coord pos);
static void split_fCoords(const fCoord* fCoords, int fCoords_size, int* xs, int* ys);
static void draw_robot_fill(int *xs, int *ys);
static void draw_robot_outline(int *xs, int *ys);

void draw_robot(const RobotRender *robot_render)
{ 
    fCoord robot_fCoords[ROBOT_SIDES];

    set_shape(robot_fCoords);
    set_angle(robot_fCoords, robot_render->angle);
    enlarge_fCoords(robot_fCoords, robot_render->size);
    set_pos(robot_fCoords, robot_render->screen_pos);       

    int xs[ROBOT_SIDES];
    int ys[ROBOT_SIDES];
    split_fCoords(robot_fCoords, ROBOT_SIDES, xs, ys);

    clear();
    draw_robot_fill(xs, ys);
    draw_robot_outline(xs, ys);
}

static void set_shape(fCoord *robot_fCoords)
{
    for (int i = 0; i < ROBOT_SIDES; i++)
    {
        robot_fCoords[i] = RobotShape[i];
    }
}

static void set_angle(fCoord* robot_fCoords, double angle) 
{
    double sin_angle = sin(angle);
    double cos_angle = cos(angle);

    for (int i = 0; i < ROBOT_SIDES; i++)
    {
        double current_x = robot_fCoords[i].x;
        double current_y = robot_fCoords[i].y;
        
        double rotated_x = current_x * cos_angle - current_y * sin_angle;
        double rotated_y = current_x * sin_angle + current_y * cos_angle;

        robot_fCoords[i] = (fCoord){rotated_x, rotated_y};
    }
}

static void enlarge_fCoords(fCoord* robot_fCoords, double scale_factor)
{
    for (int i = 0; i < ROBOT_SIDES; i++)
    {
        robot_fCoords[i].x *= scale_factor;
        robot_fCoords[i].y *= scale_factor; 
    }
}

static void set_pos(fCoord* robot_fCoords, Coord pos)
{
    for (int i = 0; i < ROBOT_SIDES; i++)
    {
        robot_fCoords[i].x += pos.x;
        robot_fCoords[i].y += pos.y;
    }
}

static void split_fCoords(const fCoord* fCoords, int fCoords_size, int* xs, int* ys)
{
    for (int i = 0; i < fCoords_size; i++)
    {
        xs[i] = round(fCoords[i].x);
        ys[i] = round(fCoords[i].y);
    }
}

static void draw_robot_fill(int *xs, int *ys)
{
    setLineWidth(ROBOT_LINE_WIDTH);
    setColour(ROBOT_COLOUR);

    fillPolygon(ROBOT_SIDES, xs, ys);
}

static void draw_robot_outline(int *xs, int *ys)
{
    setLineWidth(ROBOT_LINE_WIDTH);
    setColour(ROBOT_COLOUR);

    drawPolygon(ROBOT_SIDES, xs, ys);

    setColour(ROBOT_OUTLINE_COLOUR);
    drawLine(xs[1], ys[1], xs[2], ys[2]);
}