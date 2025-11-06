#include "draw_grid.h"

#define LINE_COLOUR black
#define MARKER_COLOUR gray
#define OBSTACLE_COLOUR black
#define EMPTY_COLOUR white
#define BORDER_COLOUR darkgray

#define LINE_WIDTH 1
#define BORDER_WIDTH 5

static Coord get_top_left(const Grid *grid, const GridView *grid_view);
static Dimensions get_grid_dimensions(const Grid *grid, const GridView *grid_view);
static void draw_background(const Grid *grid, const GridView *grid_view);
static void fill_tile(const Grid *grid, const GridView *grid_view, TilePosition tile_pos, colour colour);
static void internal_draw_marker(const Grid *grid, const GridView *grid_view, TilePosition marker_pos);
static void internal_draw_obstacle(const Grid *grid, const GridView *grid_view, TilePosition obstacle_pos);
static void internal_draw_empty(const Grid *grid, const GridView *grid_view, TilePosition empty_pos);
static void draw_tiles(const Grid *grid, const GridView *grid_view);
static void draw_grid_lines(const Grid *grid, const GridView *grid_view);
static void draw_grid_border(const Grid *grid, const GridView *grid_view);

Coord get_tile_coord(const Grid *grid, const GridView *grid_view, TilePosition tile_pos)
{
    Coord top_left = get_top_left(grid, grid_view);

    int tile_coord_x = (int)round(top_left.x + tile_pos.column * grid_view->tile_size + grid_view->tile_size / 2.0);
    int tile_coord_y = (int)round(top_left.y + tile_pos.row * grid_view->tile_size + grid_view->tile_size / 2.0);

    return (Coord){tile_coord_x, tile_coord_y};
}

void draw_marker(const Grid *grid, const GridView *grid_view, TilePosition marker_pos)
{
    background();
    internal_draw_marker(grid, grid_view, marker_pos);
    foreground();
}

void draw_obstacle(const Grid *grid, const GridView *grid_view, TilePosition obstacle_pos)
{
    background();
    internal_draw_obstacle(grid, grid_view, obstacle_pos);
    foreground();
} 

void draw_empty(const Grid *grid, const GridView *grid_view, TilePosition empty_pos)
{
    background();
    internal_draw_empty(grid, grid_view, empty_pos);
    foreground();
}

void draw_grid(const Grid *grid, const GridView *grid_view)
{
    background();
    draw_background(grid, grid_view);
    draw_grid_lines(grid, grid_view);
    draw_grid_border(grid, grid_view);
    draw_tiles(grid, grid_view);
    foreground();
}

static Coord get_top_left(const Grid *grid, const GridView *grid_view)
{
    Coord screen_centre = {grid_view->screen.width / 2.0, grid_view->screen.height / 2.0};

    int top_left_x = (int)round(screen_centre.x - grid->columns * grid_view->tile_size / 2.0);
    int top_left_y = (int)round(screen_centre.y - grid->rows * grid_view->tile_size / 2.0);

    return (Coord){top_left_x, top_left_y};
}

static Dimensions get_grid_dimensions(const Grid *grid, const GridView *grid_view)
{
    int grid_width = grid->columns * grid_view->tile_size;
    int grid_height = grid->rows * grid_view->tile_size;
    
    return (Dimensions){grid_width, grid_height};
}

static void draw_background(const Grid *grid, const GridView *grid_view)
{
    setColour(EMPTY_COLOUR);
    setLineWidth(LINE_WIDTH);

    Coord top_left = get_top_left(grid, grid_view);
    Dimensions grid_dims = get_grid_dimensions(grid, grid_view);

    fillRect(top_left.x, top_left.y, grid_dims.width, grid_dims.height);
}

static void fill_tile(const Grid *grid, const GridView *grid_view, TilePosition tile_pos, colour colour)
{
    setColour(colour);
    setLineWidth(LINE_WIDTH);
    int pad = round(grid_view->tile_size/5.0);

    Coord tile_centre = get_tile_coord(grid, grid_view, tile_pos);
    Coord tile_top_left = {tile_centre.x - grid_view->tile_size / 2.0, tile_centre.y - grid_view->tile_size / 2.0};

    fillRect(tile_top_left.x + pad, tile_top_left.y + pad, grid_view->tile_size - 2*pad, grid_view->tile_size - 2*pad);
}

static void internal_draw_marker(const Grid *grid, const GridView *grid_view, TilePosition marker_pos)
{
    fill_tile(grid, grid_view, marker_pos, MARKER_COLOUR);
}

static void internal_draw_obstacle(const Grid *grid, const GridView *grid_view, TilePosition obstacle_pos)
{
    fill_tile(grid, grid_view, obstacle_pos, OBSTACLE_COLOUR);
} 

static void internal_draw_empty(const Grid *grid, const GridView *grid_view, TilePosition empty_pos)
{
    fill_tile(grid, grid_view, empty_pos, EMPTY_COLOUR);
}

static void draw_tiles(const Grid *grid, const GridView *grid_view)
{
    for (int r = 0; r < grid->rows; r++)
    {
        for (int c = 0; c < grid->columns; c++)
        {
            switch (grid->grid_layout[r][c])
            {
                case EMPTY: internal_draw_empty(grid, grid_view, (TilePosition){r,c}); break;
                case MARKER: internal_draw_marker(grid, grid_view, (TilePosition){r,c}); break;
                case OBSTACLE: internal_draw_obstacle(grid, grid_view, (TilePosition){r,c}); break;
            }
        }
    }
}

static void draw_grid_lines(const Grid *grid, const GridView *grid_view) 
{
    setColour(LINE_COLOUR);
    setLineWidth(LINE_WIDTH);

    Coord top_left = get_top_left(grid, grid_view);
    Dimensions grid_dims = get_grid_dimensions(grid, grid_view);

    for (int r = 0; r <= grid->rows; r++)
    {
        int y_offset = r * grid_view->tile_size;
        drawLine(top_left.x, top_left.y + y_offset, top_left.x + grid_dims.width, top_left.y + y_offset);
    }
    for (int c = 0; c <= grid->columns; c++)
    {
        int x_offset = c * grid_view->tile_size;
        drawLine(top_left.x + x_offset, top_left.y, top_left.x + x_offset, top_left.y + grid_dims.height);
    }
}

static void draw_grid_border(const Grid *grid, const GridView *grid_view)
{
    setColour(BORDER_COLOUR);
    setLineWidth(2*BORDER_WIDTH);

    Coord top_left = get_top_left(grid, grid_view);
    Dimensions grid_dims = get_grid_dimensions(grid, grid_view);

    int width_pad = LINE_WIDTH + grid_dims.width;
    int height_pad = LINE_WIDTH + grid_dims.height;

    drawRect(top_left.x - BORDER_WIDTH, top_left.y - BORDER_WIDTH, width_pad + 2*BORDER_WIDTH, height_pad + 2*BORDER_WIDTH);
}