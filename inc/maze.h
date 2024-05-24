#ifndef MAZE_H
#define MAZE_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define FIELD_OF_VIEW 60
#define GRID_INTERVAL 128
#define MAP_WIDTH 30
#define MAP_HEIGHT 30
#define PI 3.14159265359f
#define MOVE_STEP 5
#define PLAYER_START_POS_X 384
#define PLAYER_START_POS_Y 3584
#define PLAYER_START_VIEW_ANGLE 45

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
 * @ACTION_BACKWARD: move backward
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
 * @MAZE_NORTH: North
 * @MAZE_EAST: East
 * @MAZE_SOUTH: South
 * @MAZE_WEST: West
 * Description: Provides a unique code representing the direction of the wall
 */
enum Wall_Direction
{
	MAZE_NORTH = 1,
	MAZE_EAST,
	MAZE_SOUTH,
	MAZE_WEST
};



/**
 * struct map_coordinate - Position on the map (x-y coordinates)
 * @x: The x-coordinate of the position
 * @y: The y-coordinate of the position
 * Description: The x and y coordinates of a point on the map
 */
typedef struct map_coordinate
{
	int x;
	int y;
} map_coordinate;

/**
 * struct MazePlayer - A player
 * @pos: The location (coordinates) of the player on the map
 * @view_angle: The viewing angle of the player
 * Description: Everything related to the player inside the maze
 */
typedef struct MazePlayer
{
	map_coordinate pos;
	float view_angle;
} MazePlayer;

/**
 * struct SDL_Instance - an SDL instance / game instance
 * @window: the game window (SDL_Window)
 * @renderer: the game renderer (SDL_Renderer)
 * @player: The game player
 * @map: The array storing the coordinates of the walls of the map
 *
 * Description: A struct representing the SDL (game) instance
 */
typedef struct SDL_Instance
{
	SDL_Window *window;
	SDL_Renderer *renderer;
	MazePlayer player;
	int map[MAP_HEIGHT][MAP_WIDTH];
	/* MazeWall map_array[GRID_SIZE]; */
} SDL_Instance;

/**
 * struct MazeRender_Column - a column (wall slice) to be rendered on screen
 * @index: the column number / position on the screen
 * @distance: the distance from the column to the player
 * @direction: the direction of the column is facing
 */
typedef struct MazeRender_Column
{
	int index;
	int distance;
	int direction;
} RenderColumn;

bool init_instance(SDL_Instance *);
int poll_events(void);
void close_instance(SDL_Instance *);
void render_graphics(SDL_Instance *);
bool init_map(int map[][MAP_WIDTH], char *filename);
bool check_for_wall(int x, int y, int map[][MAP_WIDTH]);
void do_action(int action, MazePlayer *, int map[][MAP_WIDTH]);
void do_turn(int action, MazePlayer *);
void do_move(int action, MazePlayer *, int map[][MAP_WIDTH]);
void init_player(MazePlayer *);
float to_radians(float angle_deg);
void getWallDistance(RenderColumn *, MazePlayer, int map[][MAP_WIDTH]);


#endif /* MAZE_H */
