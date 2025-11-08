#include "robot_actions.h"

static const int FRAMES_PER_ANIM = 10;

static double principal_angle(double theta);
static void update_rotation(RobotRender *robot);
static void update_pos(RobotRender *robot);
static void turn(Robot *robot, RobotRender *robot_render, int left_or_right);

bool robot_update_animation(RobotRender *robot)
{
    Animation *anim = &robot->anim;
    if (anim->curr_frame == anim->frame_count)
    {
        robot->screen_pos = anim->end_pos;
        robot->angle = direction_angles[anim->end_dir];
        return true;
    }
    else 
    {
        anim->curr_frame++; 
        update_pos(robot);
        update_rotation(robot);
        return false;
    }
}

bool robot_can_move_forward(Robot *robot, Grid *grid)
{
    TilePosition tile_ahead = get_tile_ahead(grid, robot->grid_pos, robot->dir);
    if (!check_tile_in_bounds(grid, tile_ahead)) {return false;}
    return get_tile(grid, tile_ahead) != OBSTACLE;
}

void robot_forward(Robot *robot, RobotRender *robot_render, Grid *grid, const GridView *grid_view)
{
    Coord start_pos = robot_render->screen_pos;
    Direction dir = robot->dir;

    TilePosition curr_grid_pos = robot->grid_pos;
    TilePosition end_grid_pos = get_tile_ahead(grid, curr_grid_pos, dir);
    robot->grid_pos = end_grid_pos;

    Coord end_pos = get_tile_coord(grid, grid_view, end_grid_pos);

    Animation forward_anim = {start_pos, end_pos, dir, dir, FRAMES_PER_ANIM, 0};
    robot_render->anim = forward_anim;
}

void robot_left(Robot *robot, RobotRender *robot_render)
{
    turn(robot, robot_render, -1);
}

void robot_right(Robot *robot, RobotRender *robot_render)
{
    turn(robot, robot_render, 1);
}

bool robot_at_marker(Robot *robot, const Grid *grid)
{
    return get_tile(grid, robot->grid_pos) == MARKER;
}

void robot_pick_up_marker(Robot *robot, Grid *grid, const GridView *grid_view)
{
    if (get_tile(grid, robot->grid_pos) == MARKER)
    {
        set_tile(grid, robot->grid_pos, EMPTY);
        draw_empty(grid, grid_view, robot->grid_pos);
    }
    robot->marker_count++;
}

void robot_drop_marker(Robot *robot, Grid *grid, const GridView *grid_view)
{
    bool robot_has_marker = robot->marker_count > 0;
    bool tile_empty = get_tile(grid, robot->grid_pos) == EMPTY;

    if (robot_has_marker && tile_empty)
    {
        set_tile(grid, robot->grid_pos, MARKER);
        draw_marker(grid, grid_view, robot->grid_pos);
    }
}

static double principal_angle(double theta)
{
    double new_theta = fmod(theta, 2*M_PI);
    if (new_theta > M_PI) {new_theta -= 2*M_PI;}
    if (new_theta <= -M_PI) {new_theta += 2*M_PI;}
    return new_theta;
}

static void update_rotation(RobotRender *robot)
{
    Animation* anim = &robot->anim;
    double progress = (double)anim->curr_frame / (double)anim->frame_count;

    double delta_angle = direction_angles[anim->end_dir] - direction_angles[anim->start_dir];
    double principal_delta_angle = principal_angle(delta_angle);

    double new_angle = direction_angles[anim->start_dir] + progress * principal_delta_angle;

    robot->angle = new_angle;
}

static void update_pos(RobotRender *robot)
{
    Animation* anim = &robot->anim;

    double progress = (double)anim->curr_frame / (double)anim->frame_count;

    double delta_x = anim->end_pos.x - anim->start_pos.x;
    double delta_y = anim->end_pos.y - anim->start_pos.y;

    double new_x = anim->start_pos.x + progress * delta_x;
    double new_y = anim->start_pos.y + progress * delta_y;

    robot->screen_pos = (Coord){new_x, new_y};
}

static void turn(Robot *robot, RobotRender *robot_render, int delta_dir)
{
    Coord pos = robot_render->screen_pos;
    Direction start_dir = robot->dir;
    Direction end_dir = (start_dir + delta_dir) % 4;

    Animation turn_anim = {pos, pos, start_dir, end_dir, FRAMES_PER_ANIM, 0};

    robot->dir = end_dir;
    robot_render->anim = turn_anim;
}