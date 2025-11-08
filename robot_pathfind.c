
#include "robot_pathfind.h"

static Node* create_node(RelativePosition pos, Direction parent_dir, Node* parent, int depth);
static Direction* trace_path(Node* end_node, int* length_out);
static bool is_goal(RobotMemory *mem, RelativePosition pos);
static void enqueue_neighbours(Node *curr_node, RobotMemory *mem, RobotMemory *shadow_mem, Queue *queue, Queue *f_queue);
static void free_nodes(Queue *f_queue);
static void free_bfs(Queue *f_queue, Queue *queue, RobotMemory *shadow_mem);

Direction* find_path_to_known(RobotMemory *mem, RelativePosition start, int *out_length)
{
    if (is_goal(mem, start)) {return NULL;}
    int mem_size = mem->rows * mem->columns;
    Queue queue = create_queue(mem_size, sizeof(Node*));
    Queue f_queue = create_queue(mem_size, sizeof(Node*));
    RobotMemory *shadow_mem = create_memory(1, 1);

    Node *start_node = create_node(start, NORTH, NULL, 0);
    enqueue_item(&queue, &start_node);
    enqueue_item(&f_queue, &start_node);
    Node *end_node = NULL;

    while (!queue_empty(&queue))
    {
        Node *curr_node;
        dequeue_item(&queue, &curr_node);
        if (is_goal(mem, curr_node->pos)) {end_node = curr_node; break;}

        enqueue_neighbours(curr_node, mem, shadow_mem, &queue, &f_queue);
    }
    if (!end_node) {free_bfs(&f_queue, &queue, shadow_mem); return NULL;}

    Direction *dir = trace_path(end_node, out_length);
    free_bfs(&f_queue, &queue, shadow_mem);
    return dir;
}

RelativePosition get_pos_ahead(RelativePosition pos, Direction direction)
{
    int row = pos.row;
    int column = pos.column;
    switch (direction)
    {
        case NORTH: row--; break;
        case EAST: column++; break;
        case SOUTH: row++; break;
        case WEST: column--; break;
        default: break;
    }
    return (RelativePosition){row, column};
}


static Node* create_node(RelativePosition pos, Direction parent_dir, Node* parent, int depth)
{
    Node *new_node = malloc(sizeof(Node));

    new_node->pos = pos;
    new_node->par_dir = parent_dir;
    new_node->parent = parent;
    new_node->depth = depth;
    return new_node;
}

static Direction* trace_path(Node* end_node, int* length_out)
{
    Direction *dir = malloc(end_node->depth * sizeof(Direction));
    Node *prev_node = end_node;
    for (int i = 0; i < end_node->depth; i++)
    {
        dir[end_node->depth - i - 1] = prev_node->par_dir;
        prev_node = prev_node->parent;
    }
    *length_out = end_node->depth;
    return dir;
}

static bool is_goal(RobotMemory *mem, RelativePosition pos)
{
    return (get_MTile_in_memory(mem, pos) == KNOWN);
}

static void enqueue_neighbours(Node *curr_node, RobotMemory *mem, RobotMemory *shadow_mem, Queue *queue, Queue *f_queue)
{
    for (int i = 0; i < DIRECTION_COUNT; i++)
    {
        Direction dir = i;
        RelativePosition pos_ahead = get_pos_ahead(curr_node->pos, dir);

        bool pos_unvisited = get_MTile_in_memory(shadow_mem, pos_ahead) == UNKNOWN;
        bool pos_ahead_empty = get_MTile_in_memory(mem, pos_ahead) == VISITED || get_MTile_in_memory(mem, pos_ahead) == KNOWN ;

        if (pos_unvisited && pos_ahead_empty)
        {
            set_MTile_in_memory(shadow_mem, pos_ahead, VISITED);
            Node *ahead_ptr = create_node(pos_ahead, dir, curr_node, curr_node->depth + 1);
            enqueue_item(queue, &ahead_ptr);
            enqueue_item(f_queue, &ahead_ptr);
        }
    }
}

static void free_nodes(Queue *f_queue)
{
    while(!queue_empty(f_queue))
    {
        Node* to_free;
        dequeue_item(f_queue, &to_free);
        free(to_free);
    }
}

static void free_bfs(Queue *f_queue, Queue *queue, RobotMemory *shadow_mem)
{
    free_nodes(f_queue);
    free_queue(f_queue);
    free_queue(queue);
    free_memory(shadow_mem);
}