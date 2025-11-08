#ifndef CONFIG_H
#define CONFIG_H

#include <stdlib.h>
#include "types.h"

extern int GRID_WIDTH;
extern int GRID_HEIGHT;
extern int ANIMATIONS_PER_SECOND;
extern Level L;

extern const Dimensions SCREEN;
extern const double GRID_SCREEN_RATIO;

extern const double FPS;
extern int FRAMES_PER_ANIM;

extern const double TICK;



void process_command_line(int argc, char **argv);

#endif