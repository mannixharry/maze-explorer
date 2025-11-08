#include "robot_memory.h"
#include <stdlib.h>
#include <string.h>

static void update_robot_memory(RobotMemory *robot_memory, MTile** data, MTile* flat_data, MemoryPosition new_origin, int rows, int columns);
static void update_row_pointers(MTile** new_data, MTile* new_flat_data, int rows, int columns);
static MTile* realloc_flat_data(MTile* flat_data, int rows, int columns);
static MTile** realloc_data(MTile** data, int rows);
static void extend_memory_north(RobotMemory *robot_memory);
static void extend_memory_south(RobotMemory *robot_memory);
static void extend_memory_east(RobotMemory *robot_memory);
static void extend_memory_west(RobotMemory *robot_memory);
static MemoryPosition get_memory_position(const RobotMemory *robot_memory, RelativePosition pos);
static bool check_in_memory_bounds(const RobotMemory *robot_memory, RelativePosition pos);
static void extend_memory_to_position(RobotMemory *robot_memory, RelativePosition pos);

void write_memory_to_file(RobotMemory *mem, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) {
        perror("Failed to open file");
        return;
    }

    fprintf(f, "%d %d\n", mem->rows, mem->columns);
    for (int r = 0; r < mem->rows; r++)
    {
        for(int c = 0; c< mem->columns; c++)
        {
            fprintf(f, "%s ", 
            mem->data[r][c] == UNKNOWN      ? "?" :
            mem->data[r][c] == M_OBSTACLE ? "O" :
            mem->data[r][c] == VISITED ? "#" :
            mem->data[r][c] == KNOWN ? "!" : "N");
        }
        fprintf(f, "\n");
    }
    fclose(f);
}


RobotMemory* create_memory(int rows, int columns)
{   
    RobotMemory *robot_memory = malloc(sizeof(RobotMemory));
    if (!robot_memory) {return NULL;}

    MTile* flat_data = malloc(rows * columns * sizeof(MTile));
    if (!flat_data) {free(robot_memory); return NULL;}
    for (int i = 0; i < rows * columns; i++)
    {
        flat_data[i] = UNKNOWN;
    }

    MTile** data = malloc(rows * sizeof(MTile *));
    if (!data) {free(robot_memory); free(flat_data); return NULL;}

    for (int r = 0; r < rows; r++)
    {
        data[r] = &flat_data[r * columns];
    }

    MemoryPosition origin_pos = {rows/2, columns/2};

    update_robot_memory(robot_memory, data, flat_data, origin_pos, rows, columns);
    return robot_memory;
}

void set_MTile_in_memory(RobotMemory *robot_memory, RelativePosition pos, MTile MTile)
{
    if (!check_in_memory_bounds(robot_memory, pos)) {extend_memory_to_position(robot_memory, pos);}
    MemoryPosition mem_pos = get_memory_position(robot_memory, pos);
    robot_memory->data[mem_pos.row][mem_pos.column] = MTile;
}

MTile get_MTile_in_memory(RobotMemory *robot_memory, RelativePosition pos)
{
    if (!check_in_memory_bounds(robot_memory, pos)) {extend_memory_to_position(robot_memory, pos);}
    MemoryPosition mem_pos = get_memory_position(robot_memory, pos);
    return robot_memory->data[mem_pos.row][mem_pos.column];
}

void free_memory(RobotMemory *robot_memory)
{
    free(robot_memory->flat_data);
    free(robot_memory->data);
    free(robot_memory);
}

static void update_robot_memory(RobotMemory* robot_memory, MTile** data, MTile* flat_data, MemoryPosition new_origin, int rows, int columns)
{
    robot_memory->data = data;
    robot_memory->flat_data = flat_data;
    robot_memory->origin_pos = new_origin;
    robot_memory->rows = rows;
    robot_memory->columns = columns;
}

static void update_row_pointers(MTile** new_data, MTile* new_flat_data, int rows, int columns)
{
    for (int r = 0; r < rows; r++)
    {
        new_data[r] = &new_flat_data[r * columns];
    }
}

static MTile* realloc_flat_data(MTile* flat_data, int rows, int columns)
{
    MTile* new_flat_data = realloc(flat_data, rows * columns * sizeof(MTile));
    if (!new_flat_data) {fprintf(stderr, "Failed to realloc flat_data"); exit(1);}
    return new_flat_data;
}

static MTile** realloc_data(MTile** data, int rows)
{
    MTile** new_data = realloc(data, rows * sizeof(MTile *));
    if (!new_data) {fprintf(stderr, "Failed to realloc data"); exit(1);} 
    return new_data;
}

static void extend_memory_north(RobotMemory* robot_memory)
{
    int old_rows = robot_memory->rows;
    int new_rows = 2 * old_rows;
    int columns = robot_memory->columns;
    MemoryPosition new_origin = robot_memory->origin_pos;
    new_origin.row += old_rows;

    MTile* new_flat_data = realloc_flat_data(robot_memory->flat_data, new_rows, columns);
    MTile** new_data = realloc_data(robot_memory->data, new_rows);

    memmove(&new_flat_data[old_rows * columns], new_flat_data, old_rows * columns * sizeof(MTile));
    for (int i = 0; i < old_rows * columns; i++) 
    {
        new_flat_data[i] = UNKNOWN;
    }

    update_row_pointers(new_data, new_flat_data, new_rows, columns);
    update_robot_memory(robot_memory, new_data, new_flat_data, new_origin, new_rows, columns);
}

static void extend_memory_south(RobotMemory* robot_memory)
{
    int old_rows = robot_memory->rows;
    int new_rows = 2 * old_rows;
    int columns = robot_memory->columns;
    MemoryPosition new_origin = robot_memory->origin_pos;

    MTile* new_flat_data = realloc_flat_data(robot_memory->flat_data, new_rows, columns);
    MTile** new_data = realloc_data(robot_memory->data, new_rows);

    for (int i = old_rows * columns; i < new_rows * columns; i++) 
    {
        new_flat_data[i ] = UNKNOWN;
    }
    update_row_pointers(new_data, new_flat_data, new_rows, columns);
    update_robot_memory(robot_memory, new_data, new_flat_data, new_origin, new_rows, columns);
}

static void extend_memory_east(RobotMemory* robot_memory)
{
    int rows = robot_memory->rows;
    int old_columns = robot_memory->columns;
    int new_columns = 2 * old_columns;
    MemoryPosition new_origin = robot_memory->origin_pos;

    MTile* new_flat_data = realloc_flat_data(robot_memory->flat_data, rows, new_columns);
    MTile** new_data = robot_memory->data;

    // Working right-to-left, copy each row to twice its index, and pad with zeroes.
    for (int r = rows-1; r >= 0; r--)
    {
        memmove(&new_flat_data[r * new_columns], &new_flat_data[r * old_columns], old_columns * sizeof(MTile));
        for (int c = old_columns; c < new_columns; c++)
        {
            new_flat_data[r * new_columns + c] = UNKNOWN;
        }
    }

    update_row_pointers(new_data, new_flat_data, rows, new_columns);
    update_robot_memory(robot_memory, new_data, new_flat_data, new_origin, rows, new_columns);
}

static void extend_memory_west(RobotMemory* robot_memory)
{
    int rows = robot_memory->rows;
    int old_columns = robot_memory->columns;
    int new_columns = 2 * old_columns;
    MemoryPosition new_origin = robot_memory->origin_pos;
    new_origin.column += old_columns;

    MTile* new_flat_data = realloc_flat_data(robot_memory->flat_data, rows, new_columns);
    MTile** new_data = robot_memory->data;
    
    // Working right-to-left, copy each row to twice its index + old_columns, and pad with zeroes.
    for (int r = rows-1; r >= 0; r--)
    {
        memmove(&new_flat_data[r * new_columns + old_columns], &new_flat_data[r * old_columns], old_columns * sizeof(MTile));
        for (int c = 0; c < old_columns; c++)
        {
            new_flat_data[r * new_columns + c] = UNKNOWN;
        }
    }

    update_row_pointers(new_data, new_flat_data, rows, new_columns);
    update_robot_memory(robot_memory, new_data, new_flat_data, new_origin, rows, new_columns);
}

static MemoryPosition get_memory_position(const RobotMemory *robot_memory, RelativePosition pos)
{
    int mem_row = pos.row + robot_memory->origin_pos.row;
    int mem_col = pos.column + robot_memory->origin_pos.column;
    return (MemoryPosition){mem_row, mem_col};
}

static bool check_in_memory_bounds(const RobotMemory *robot_memory, RelativePosition pos)
{
    int c = robot_memory->columns;
    int r = robot_memory->rows;
    MemoryPosition mem_pos = get_memory_position(robot_memory, pos);
    return (0 <= mem_pos.row && mem_pos.row < r && 0 <= mem_pos.column && mem_pos.column < c);
}

static void extend_memory_to_position(RobotMemory *robot_memory, RelativePosition pos)
{
    while (get_memory_position(robot_memory, pos).column >= robot_memory->columns)
    {
        extend_memory_east(robot_memory);
    }
    while (get_memory_position(robot_memory, pos).column < 0)
    {
        extend_memory_west(robot_memory);
    }
    while (get_memory_position(robot_memory, pos).row >= robot_memory->rows)
    {
        extend_memory_south(robot_memory);
    }
    while (get_memory_position(robot_memory, pos).row < 0)
    {
        extend_memory_north(robot_memory);
    }
}