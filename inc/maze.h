#ifndef MAZE_H
#define MAZE_H

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define FIELD_OF_VIEW 60
#define GRID_INTERVAL 128
#define WALL_HEIGHT GRID_INTERVAL
#define MAP_WIDTH 30
#define MAP_HEIGHT 30
#define PI 3.14159265359f

#define MOVE_STEP 5
#define ROTATION_STEP 1 /* Degrees */
#define PLAYER_START_POS_X 512
#define PLAYER_START_POS_Y 3554
#define PLAYER_START_VIEW_ANGLE 210
#define MAX_ACTIONS 10 /* Maximum number of simultaneous actions to be polled*/

#define Y_DIRECTION_UP -1
#define Y_DIRECTION_DOWN 1
#define Y_DIRECTION_NONE 0
#define X_DIRECTION_LEFT -1
#define X_DIRECTION_RIGHT 1
#define X_DIRECTION_NONE 0

/**
 * enum Wall_Type - The wall type (used to select the appropriate wall texture)
 * @DEFAULT_WALL: Normal maze wall
 * @START_1: Starting line wall (left half)
 * @START_2: Starting line wall (right half)
 * @FINISH_1: Finish line wall (left half)
 * @FINISH_2: Finish line wall (right half)
 * @TOTAL_WALL_TYPES: Total number of wall types
 */
enum Wall_Type
{
	DEFAULT_WALL,
	START_1,
	START_2,
	FINISH_1,
	FINISH_2,
	TOTAL_WALL_TYPES
};

/**
 * enum Action_Code - action codes corresponding to an event
 * @ACTION_QUIT: Quit the game
 * @ACTION_NONE: No action required
 * @ACTION_UP: Move/pan/rotate up
 * @ACTION_DOWN: Move/pan/rotate down
 * @ACTION_TURN_LEFT: rotate/turn left
 * @ACTION_TURN_RIGHT: rotate/turn right
 * @ACTION_MOVE_FORWARD: move forward
 * @ACTION_MOVE_BACKWARD: move backward
 * @ACTION_MOVE_LEFT: pan/move left
 * @ACTION_MOVE_RIGHT: pan/move right
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
	ACTION_TURN_LEFT,
	ACTION_TURN_RIGHT,
	ACTION_MOVE_FORWARD,
	ACTION_MOVE_BACKWARD,
	ACTION_MOVE_LEFT,
	ACTION_MOVE_RIGHT
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
 * struct M_Texture - A texture struct
 * @mTexture: The SDL Texture
 * @width: The texture's width
 * @height: The texture's height
 */
typedef struct M_Texture
{
	SDL_Texture *mTexture;
	int width;
	int height;
} M_Texture;

/**
 * struct SDL_Instance - an SDL instance / game instance
 * @window: the game window (SDL_Window)
 * @renderer: the game renderer (SDL_Renderer)
 * @player: The game player
 * @map: The array storing the coordinates of the walls of the map
 * @wall_texture: A wall texture
 * @floor_texture: A floor texture
 * @ceiling_texture: A ceiling texture
 * Description: A struct representing the SDL (game) instance
 */
typedef struct SDL_Instance
{
	SDL_Window *window;
	SDL_Renderer *renderer;
	MazePlayer player;
	M_Texture wall_texture[TOTAL_WALL_TYPES];
	M_Texture floor_texture;
	M_Texture ceiling_texture;
	int map[MAP_HEIGHT][MAP_WIDTH];
} SDL_Instance;

/**
 * struct RendColumn - a column (wall slice) to be rendered on screen
 * @index: the column number / position on the screen
 * @ray_angle: the ray angle of the column
 * @distance: the distance from the column to the player
 * @direction: the direction of the column is facing
 * @wall_pos: position on the wall block (from 0 to GRID interval length)
 * @wb_row: wall base point (row)
 * @type: the wall type (used to select the correct texture)
 */
typedef struct RendColumn
{
	int index;
	int ray_angle;
	int distance;
	int direction;
	int wall_pos;
	int wb_row;
	int type;
} RendColumn;

bool init_instance(SDL_Instance *);
bool poll_events(int actions[]);
void close_instance(SDL_Instance *);
void render_graphics(SDL_Instance *);
bool init_map(int map[][MAP_WIDTH], char *filename);
bool check_for_wall(int x, int y, int map[][MAP_WIDTH]);
void do_action(int action[], MazePlayer *, int map[][MAP_WIDTH]);
void do_turn(int action, MazePlayer *);
void do_move(int action, MazePlayer *, int map[][MAP_WIDTH]);
void init_player(MazePlayer *);
float to_radians(float angle_deg);
int fix_distortion(float distance, float view_angle, float ray_angle);
void getWallDistance(RendColumn *, MazePlayer, int map[][MAP_WIDTH]);
void init_texture(M_Texture *texture);
void drawWallSlice(RendColumn *, int, SDL_Instance *);
void renderWallTexture(SDL_Instance *gInstance, int x, int y, int height,
	RendColumn column);
bool loadTexture(SDL_Renderer *gRenderer, M_Texture *texture,
	char *filepath);
void clear_actions(int actions[]);
void drawFloorSlice(SDL_Instance *gInstance, RendColumn column,
	MazePlayer player, int pp_distance);
void paintFloor(SDL_Renderer *gRenderer);

#endif /* MAZE_H */
