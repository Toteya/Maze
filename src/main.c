#include <SDL2/SDL.h>
#include "../inc/maze.h"

/**
 * main - Entry point to the Maze game
 * A player navigates through a maze and tries to overcomes obstacles
 * @argc: Number of command line arguments
 * @argv: Array of command line arguments
 * Return: 0 (SUCCESS)
 */
int main(int argc, char *argv[])
{
	bool quit = false;
	SDL_Instance gInstance;

	(void) argv;
	(void) argc;

	/* Initialise game instance */
	if (init_instance(&gInstance) == false)
	{
		return (EXIT_FAILURE);
	}

	/* Todo: load media */

	/* Start game loop */
	while (!quit)
	{
		if (poll_events() < 0)
			quit = true;

		/* Todo: Render */
	}

	close_instance(&gInstance);

	return (EXIT_SUCCESS);
}

