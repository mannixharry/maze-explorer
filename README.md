# COMP0002CW1

Robot Maze

## Instructions for compilation and use

To compile the program (using GCC), run:

```
gcc main.c graphics.c draw_grid.c draw_robot.c grid.c robot_actions.c levels.c helper.c simulation.c robot_memory.c queue.c robot_pathfind.c config.c -lm
```

To run:

The program accepts config parameters in a command line input (see config.c). There are also other adjustable parameters in the config.c file.

To run in default settings use:
```
./a.out | java -jar drawapp-4.5.jar
```

To specify parameters, use:
```
./a.out {rows} {columns} {level} {animations_per_second} | java -jar drawapp-4.5.jar
```

* `{rows}` and `{columns}` specify the dimensions of the arena (grid) drawn on the screen.
* `{level}` can take values 1 to 5, and determines the Stage (see coursework specification) to simulate. Default level is 4.
* `{animation_per_second}`. Default value is 10. Adjust to vary the robot's speed.

## Example command line prompts

30x20 random maze (level 4), default:
```
./a.out 30 20 4 10 | java -jar drawapp-4.5.jar
```

50x50 circular arena (level 5), extremely fast:
```
./a.out 50 50 5 60 | java -jar drawapp-4.5.jar
```

20x20 circular arena (level 5), default pace:
```
./a.out 20 20 5 10 | java -jar drawapp-4.5.jar
```

5x5 random maze (level 4), slow:
```
./a.out 5 5 4 3 | java -jar drawapp-4.5.jar
```

10x10 empty maze (levels 1 & 2), default pace:
```
./a.out 10 10 1 10 | java -jar drawapp-4.5.jar
```

5x10 empty maze (level 3), fast pace:
```
./a.out 5 10 3 20 | java -jar drawapp-4.5.jar
```

## Program key features (non-exhaustive)

* Options for empty, maze-like, and circular arenas (Stages 1 to 5)
* Robot algorithm searches the entire space (always finds all the markers), see (sim.c)
* The number of markers is random, and the robot is not told how many there are.
* Random maze generation (see stage 4). (BFS ensures every tile is reachable.)
* Robot starts with no information (besides its cardinal direction). It does not know the grid size, obstacle positions, position in the grid, etc.
* Robot is animated to move and rotate smoothly (a slower pace can make this more noticeable)

## Project structure

There are many files to this project, but they generally divide between the logical representation of the robot in the arena, ie the robot search algorithm (see robot_actions.c, robot_memory.c, robot_pathfind.c, simulation.c etc), and the graphical representation of the grid and robot animations (see draw_robot.c, draw_grid.c, levels.c etc). There are also a few helper .h and .c files (such as queue.c and helper.c). I have tried to maintain a strong abstraction between these modules in my code.

At a first glance, the robot's algorithm for searching the maze may seem unnecessarily complex. This is mainly due to the fact that I decided to give my robot no information about the arena it lives in. It does not know the size of the grid, or its starting position in the grid; it dynamically builds up a picture of its surroundings as it explores the maze (this is the robot's memory). robot_memory.txt updates every time the program runs, and looking at this gives insight into the robot's "view" of the maze. Since the robot is unaware of the positions of obstacles in the maze, and can only observe the tile ahead (through the can_move_forward function), the robot has to spin around a lot (constantly) to map out the maze. On a higher speed, these turns can be hard to track, so I recommend running the program at least once on a slow pace to see what is really going on.

The fact that the robot does not know the dimensions of the grid led me to implement an interesting memory system, where extra memory is dynamically allocated (using realloc) to store features of the maze as they are discovered. In theory, while graphics.c does not support this (for a lack of infinite pixels), this would allow my robot to safely explore areas of arbitrary size (assuming space on heap does not run out of course).

Here are the contents of robot_memory after exploring the 5x5 arena given in Example.png:

```
8 8
? ? ? ? ? ? ? ? 
? ? ? ? O O ? ? 
? ? ? O # # O ? 
? O O # # # O ? 
O ! # # O # O ? 
? O # # # # O ? 
? O # O # # O ? 
? ? O ? O O ? ? 
```

Key:

* `?` Unknown
* `O` Obstacle
* `#` Explored
* `!` Known (also the end tile, see simulation.c)

Notice that the robot held an 8x8 array of information in memory; for a larger grid, the size of the memory used expands.
