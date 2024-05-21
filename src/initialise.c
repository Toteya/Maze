#include "../inc/maze.h"

/**
 * init_instance - Creates and initialises an SDL instance
 * @gInstance: The SDL game instance to be intialised
 * Return: bool - true (SUCCESS) or false (FALSE)
 */
bool init_instance(SDL_Instance *gInstance)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		fprintf(stderr, "Unable to initialise SDL! Error: %s\n",
			SDL_GetError()
		);
	}

	gInstance->window = SDL_CreateWindow(
		"The Maze",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_SHOWN
	);
	if (gInstance->window == NULL)
	{
		fprintf(stderr, "Unable to create SDL_Window! Error: %s\n",
			SDL_GetError()
		);
		SDL_Quit();
		return (false);
	}
	gInstance->renderer = SDL_CreateRenderer(
		gInstance->window,
		-1,
		SDL_RENDERER_ACCELERATED
	);
	if (gInstance->renderer == NULL)
	{
		printf("Unable to create SDL_Renderer! Error: %s\n",
			SDL_GetError());
		SDL_DestroyWindow(gInstance->window);
		SDL_Quit();
		return (false);
	}
	/* Set renderer colour */
	SDL_SetRenderDrawColor(gInstance->renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	return (true);
}

/**
 * close_instance - Destroys/frees an SDL_Instance
 * @gInstance: the SDL game instance to be destroyed
 * Return: Nothing.
 */
void close_instance(SDL_Instance *gInstance)
{
	SDL_DestroyRenderer(gInstance->renderer);
	SDL_DestroyWindow(gInstance->window);
	SDL_Quit();
}