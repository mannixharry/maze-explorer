#include "robot_actions.h"
const int frames_per_move = 60;

double principle_angle(double theta)
{
    double new_theta = fmod(theta, 2*M_PI);
    if (new_theta > M_PI) {new_theta -= 2*M_PI;}
    if (new_theta <= -M_PI) {new_theta += 2*M_PI;}
    return new_theta;
}

void update_rotation(RobotRender* robot)
{
    Animation* animation = &robot->animation;
    double progress = (double)animation->current_frame / (double)animation->number_of_frames;

    double delta_theta = direction_angles[animation->end_direction] - direction_angles[animation->start_direction];
    double principle_delta_theta = principle_angle(delta_theta);

    double new_theta = direction_angles[animation->start_direction] + progress * principle_delta_theta;

    robot->angle = new_theta;
}

void update_position(RobotRender* robot)
{
    Animation* animation = &robot->animation;

    double progress = (double)animation->current_frame / (double)animation->number_of_frames;

    double delta_x = animation->end_position.x - animation->start_position.x;
    double delta_y = animation->end_position.y - animation->start_position.y;

    double new_x = animation->start_position.x + progress * delta_x;
    double new_y = animation->start_position.y + progress * delta_y;

    robot->screen_position = (Coord){new_x, new_y};
}

bool update_animation(RobotRender* robot)
{
    Animation* animation = &robot->animation;
    if (animation->current_frame == animation->number_of_frames)
    {
        robot->screen_position = animation->end_position;
        robot->angle = direction_angles[animation->end_direction];
        return true;
    }
    else 
    {
        animation->current_frame++; 
        update_position(robot);
        update_rotation(robot);

        return false;
    }
}

void forward(Robot* robot, Grid *grid, GridView* grid_view)
{
    Coord start_position = robot->robot_render.screen_position;
    Direction direction = robot->direction;

    TilePosition current_grid_position = robot->grid_position;
    TilePosition end_grid_position = get_tile_ahead(grid, current_grid_position, direction);
    robot->grid_position = end_grid_position;

    Coord end_position = get_tile_coord(grid, grid_view, end_grid_position);

    Animation forward_animation = {start_position, end_position, direction, direction, frames_per_move, 0};
    robot->robot_render.animation = forward_animation;
}

void turn(Robot *robot, int delta)
{
    Coord position = robot->robot_render.screen_position;
    Direction start_direction = robot->direction;
    Direction end_direction = (start_direction + delta) % 4;

    Animation left_animation = {position, position, start_direction, end_direction, frames_per_move, 0};

    robot->direction = end_direction;
    robot->robot_render.animation = left_animation;
}

void right(Robot *robot)
{
    turn(robot, 1);
}

void left(Robot *robot)
{
    turn(robot, -1);
}

bool at_marker(Robot* robot, Grid *grid)
{
    int r = robot->grid_position.row;
    int c = robot->grid_position.column;

    if (grid->grid_layout[r][c] == MARKER)
    {
        return true;
    }
    return false; 
}

bool can_move_forward(Robot *robot, Grid *grid)
{
    TilePosition coord_ahead = get_tile_ahead(grid, robot->grid_position, robot->direction);
    int r = coord_ahead.row;
    int c = coord_ahead.column;

    if (r < 0 || r >= grid->rows || c < 0 || c >= grid->columns)
    {
        return false;
    }
    if (grid->grid_layout[r][c] == OBSTACLE)
    {
        return false;
    }

    return true; 
}

void pick_up_marker(Robot *robot, Grid *grid, GridView *grid_view)
{
    int r = robot->grid_position.row;
    int c = robot->grid_position.column;

    if (grid->grid_layout[r][c] == MARKER)
    {
        grid->grid_layout[r][c] = EMPTY;
        draw_empty(grid, grid_view, robot->grid_position);
    }

    robot->marker_count++;
}

void drop_marker(Robot *robot, Grid *grid, GridView *grid_view)
{
    int r = robot->grid_position.row;
    int c = robot->grid_position.column;

    if (grid->grid_layout[r][c] == EMPTY && robot->marker_count > 0) 
    {
        grid->grid_layout[r][c] = MARKER;
        draw_marker(grid, grid_view, robot->grid_position);
    }
}