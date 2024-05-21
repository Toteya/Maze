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
    Maze_wall_block map_array[GRID_SIZE];

	(void) argv;
	(void) argc;

	/* Initialise game instance */
	if (init_instance(&gInstance) == false)
	{
		return (EXIT_FAILURE);
	}

    if (init_map(map_array) == false)
    {
        fprintf(stderr, "Map failed to initialise\n");
        return (EXIT_FAILURE);
    }

	/* Todo: load media */

	/* Start game loop */
	while (!quit)
	{
		if (poll_events() < 0)
			quit = true;

		/* Todo: Render */
	    render_graphics(gInstance.renderer, map_array);
	}

	close_instance(&gInstance);

	return (EXIT_SUCCESS);
}
