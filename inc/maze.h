#ifndef MAZE_H
#define MAZE_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define FIELD_OF_VIEW 60
#define GRID_INTERVAL 64
#define GRID_SIZE 600 /* Map size: 30 x 30 */
#define PI 3.14159265359f
#define MOVE_STEP 5

#define Y_DIRECTION_UP -1
#define Y_DIRECTION_DOWN 1
#define Y_DIRECTION_NONE 0
#define X_DIRECTION_LEFT -1
#define X_DIRECTION_RIGHT 1
#define X_DIRECTION_NONE 0




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
	ACTION_BACKWARD
};

/**
 * enum Wall_Direction - The direction that a wall is facing
 * @NORTH: North
 * @EAST: East
 * @SOUTH: South
 * @WEST: West
 * Description: Provides a unique code representing the direction of the wall
 */
enum Wall_Direction
{
    NORTH = 1,
    EAST,
    SOUTH,
    WEST
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
 * struct Maze_wall_block - A wall block (cube) in the map
 * @x: The x-coordinate on the grid
 * @y: the y-coordinate on the grid
 * Description: The building block cube of a wall on the map
 */
typedef struct Maze_wall_block
{
	int x;
	int y;
} Maze_wall_block;

/**
 * struct Maze_player - A player
 * @pos: The location (coordinates) of the player on the map
 * @view_angle: The viewing angle of the player
 * Description: Everything related to the player inside the maze
 */
typedef struct Maze_player
{
	map_location pos;
	float view_angle;
} Maze_player;

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
    Maze_player player;
    Maze_wall_block map_array[GRID_SIZE];
} SDL_Instance;

/**
 * struct MazeRender_column - a column (wall slice) to be rendered on the screen
 * @index: the column number / position on the screen
 * @distance: the distance from the column to the player
 * @direction: the direction of the column is facing
 */
typedef struct Render_column
{
    int index;
    int distance;
    int direction;
} MazeRender_column;

bool init_instance(SDL_Instance *);
int poll_events(void);
void close_instance(SDL_Instance *);
void render_graphics(SDL_Instance *);
bool init_map(Maze_wall_block []);
void do_action(int action, Maze_player *);
void do_turn(int action, Maze_player *);
void do_forward(Maze_player *player);
void init_player(Maze_player *);
float to_radians(float angle_deg);


#endif /* MAZE_H */
