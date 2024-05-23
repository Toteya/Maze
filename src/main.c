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
	/* bool quit = false; */
	int action;
	SDL_Instance gInstance;
	/* Maze_wall_block map_array[GRID_SIZE]; */

	(void) argv;
	(void) argc;

	/* Initialise game instance */
	if (init_instance(&gInstance) == false)
	{
		return (EXIT_FAILURE);
	}

	if (init_map(gInstance.map) == false)
	{
		fprintf(stderr, "Map failed to initialise\n");
		return (EXIT_FAILURE);
	}

	init_player(&(gInstance.player));

	/* Todo: load media */

	/* Start game loop */
	while (true)
	{
		action = poll_events();
		if (action < 0)
			break;
		do_action(action, &(gInstance.player), gInstance.map);

		render_graphics(&gInstance);
	}

	close_instance(&gInstance);

	return (EXIT_SUCCESS);
}
