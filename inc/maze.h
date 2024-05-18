#ifndef MAZE_H
#define MAZE_H

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

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

bool init_instance(SDL_Instance *);
int poll_events(void);
void close_instance(SDL_Instance *);

#endif /* MAZE_H */
