#ifndef MAZE_H
#define MAZE_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define FIELD_OF_VIEW 60
#define GRID_WIDTH 64
#define GRID_HEIGHT 64
#define PI 3.14159265359f

#define Y_DIRECTION_UP -1
#define Y_DIRECTION_DOWN 1
#define Y_DIRECTION_NONE 0
#define X_DIRECTION_LEFT -1
#define X_DIRECTION_RIGHT 1
#define X_DIRECTION_NONE 0


/**
 * struct SDL_Instance - an SDL instance / game instance
 * @window: the game window (SDL_Window)
 * @renderer: the game renderer (SDL_Renderer)
 *
 * Description: A struct representing the SDL (game) instance
 */
typedef struct SDL_Instance
{
	SDL_Window *window;
	SDL_Renderer *renderer;
} SDL_Instance;

/**
 * enum Action_Code - action codes corresponding to an event
 * @ACTION_QUIT: Quit the game
 * @ACTION_NONE: No action required
 * @ACTION_UP: Move/pan/rotate up
 * @ACTION_DOWN: Move/pan/rotate down
 * @ACTION_LEFT: Move/pan/rotate left
 * @ACTION_RIGHT: Move/pan/rotate right
 * @ACTION_FORWARD: move forward
 * @ACTION_BACKWORD: move backward
 *
 * Description: The enum provides unique action codes for the action
 * to be taken that corresponds to an event that has been polled
 */
enum Action_Code
{
	ACTION_QUIT = -1,
	ACTION_NONE,
	ACTION_UP,
	ACTION_DOWN,
	ACTION_LEFT,
	ACTION_RIGHT,
	ACTION_FORWARD,
	ACTION_BACKWORD
};

/**
 * struct map_location - Position on the map (x-y coordinates)
 * @x: The x-coordinate of the position
 * @y: The y-coordinate of the position
 * Description: The x and y coordinates of a point on the map
 */
typedef struct map_location
{
    int x;
    int y;
} map_location;

/**
 * struct wall_block - A wall block (cube) in the map
 * @x: The x-coordinate on the grid
 * @y: the y-coordinate on the grid
 * Description: The building block cube of a wall on the map
 */
typedef struct wall_block
{
    int x;
    int y;
} wall_block;

/**
 * struct player - A player
 * @pos: The location (coordinates) of the player on the map
 * @view_angle: The viewing angle of the player
 * Description: Everything related to the player inside the maze
 */
typedef struct Maze_player
{
    map_location pos;
    float view_angle;
} Maze_player;



bool init_instance(SDL_Instance *);
int poll_events(void);
void close_instance(SDL_Instance *);
void render_graphics(void);

#endif /* MAZE_H */
