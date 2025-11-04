#include "draw_grid.h"

Coord get_top_left(Grid *grid, int cell_size, Screen screen)
{
    const Coord screen_centre = {screen.width / 2.0, screen.height / 2.0};
    const int top_left_x = round(screen_centre.x - grid->columns * cell_size / 2.0);
    const int top_left_y = round(screen_centre.y - grid->rows * cell_size / 2.0);

    const Coord top_left = {top_left_x, top_left_y};
    return top_left;
}

void draw_grid(Grid *grid, GridView *grid_view)
{
    draw_grid_lines(grid, grid_view);
    draw_grid_border(grid, grid_view);
    draw_grid_cells(grid, grid_view);
}

void draw_grid_cells(Grid *grid, GridView *grid_view)
{
    for (int r = 0; r < grid->rows; r++)
    {
        for (int c = 0; c < grid->columns; c++)
        {
            switch (grid->grid_layout[r][c])
            {
                case EMPTY: draw_empty(grid, grid_view, (GridPosition){r,c}); break;
                case MARKER: draw_marker(grid, grid_view, (GridPosition){r,c}); break;
                case OBSTACLE: draw_obstacle(grid, grid_view, (GridPosition){r,c}); break;
            }
        }
    }
}

void fill_grid_cell(Grid *grid, GridView *grid_view, GridPosition grid_position, colour colour)
{
    // Update to remove magic numbers

    background();
    const int w = 5; // Line thickness
    Coord cell_centre = get_grid_cell_coordinate(grid, grid_view, grid_position);
    Coord cell_top_left = {cell_centre.x - grid_view->cell_size/2, cell_centre.y - grid_view->cell_size/2};
    setColour(colour);
    fillRect(cell_top_left.x + w, cell_top_left.y + w, grid_view->cell_size - 2*w, grid_view->cell_size - 2*w);
    foreground();
}

void draw_marker(Grid *grid, GridView *grid_view, GridPosition marker_position)
{
    fill_grid_cell(grid, grid_view, marker_position, pink);
}

void draw_obstacle(Grid *grid, GridView *grid_view, GridPosition obstacle_position)
{
    fill_grid_cell(grid, grid_view, obstacle_position, black);
} 

void draw_empty(Grid *grid, GridView *grid_view, GridPosition marker_position)
{
    fill_grid_cell(grid, grid_view, marker_position, white);
}

void draw_grid_lines(Grid *grid, GridView *grid_view) 
{
    Coord top_left = get_top_left(grid, grid_view->cell_size, grid_view->screen);

    int grid_width = grid->columns * grid_view->cell_size;
    int grid_height = grid->rows * grid_view->cell_size;

    for (int r = 0; r <= grid->rows; r++)
    {
        int y_offset = r * grid_view->cell_size;
        drawLine(top_left.x, top_left.y + y_offset, top_left.x + grid_width, top_left.y + y_offset);
    }
    for (int c = 0; c <= grid->columns; c++)
    {
        int x_offset = c * grid_view->cell_size;
        drawLine(top_left.x + x_offset, top_left.y, top_left.x + x_offset, top_left.y + grid_height);
    }
}

Coord get_grid_cell_coordinate(Grid *grid, GridView *grid_view, GridPosition grid_position)
{
    Coord top_left = get_top_left(grid, grid_view->cell_size, grid_view->screen);

    int screen_x = top_left.x + grid_position.column * grid_view->cell_size + grid_view->cell_size / 2;
    int screen_y = top_left.y + grid_position.row * grid_view->cell_size + grid_view->cell_size / 2;

    return (Coord){screen_x, screen_y};
}

void draw_grid_border(Grid *grid, GridView *grid_view)
{
    Coord top_left = get_top_left(grid, grid_view->cell_size, grid_view->screen);

    const int grid_width = grid->columns * grid_view->cell_size;
    const int grid_height = grid->rows * grid_view->cell_size;

    setColour(red);
    setLineWidth(5);
    drawRect(top_left.x, top_left.y, grid_width, grid_height);
    setLineWidth(1);
}