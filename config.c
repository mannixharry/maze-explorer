#include "config.h"

int GRID_WIDTH = 20;
int GRID_HEIGHT = 20;
Level L = FOUR;
int ANIMATIONS_PER_SECOND = 25;

int FRAMES_PER_ANIM = 30; 
const Dimensions SCREEN = {800, 800};

const double GRID_SCREEN_RATIO = 0.85;
const double FPS = 60;
const double TICK = 1000/FPS;

void process_command_line(int argc, char **argv)
{
    if (argc == 5)
    {
        GRID_WIDTH = atoi(argv[1]);
        GRID_HEIGHT = atoi(argv[2]);
        L = (Level)atoi(argv[3]);
        ANIMATIONS_PER_SECOND = atoi(argv[4]);

        FRAMES_PER_ANIM = (int)(FPS / ANIMATIONS_PER_SECOND);
        if (FRAMES_PER_ANIM < 1) FRAMES_PER_ANIM = 1;
    }
}
