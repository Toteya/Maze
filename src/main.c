#include <SDL2/SDL.h>
#include "../inc/maze.h"


void game_loop(SDL_Instance gInstance);

/**
 * main - Entry point to the Maze game
 * A player navigates through a maze and tries to overcomes obstacles
 * @argc: Number of command line arguments
 * @argv: Array of command line arguments
 * Return: 0 (SUCCESS)
 */
int main(int argc, char *argv[])
{
	SDL_Instance gInstance;
	char *map_filename = "maze_map.csv";

	(void) argv;
	(void) argc;

	/* Initialise game instance */
	if (init_instance(&gInstance) == false)
	{
		return (EXIT_FAILURE);
	}

	if (init_map(gInstance.map, map_filename) == false)
	{
		fprintf(stderr, "Map failed to initialise\n");
		return (EXIT_FAILURE);
	}

	init_player(&(gInstance.player));

	/* TODO: load media */

	game_loop(gInstance);

	close_instance(&gInstance);

	return (EXIT_SUCCESS);
}

/**
 * game_loop - The main game loop
 * @gInstance: The maze game SDL instance
 * Return: Nothing
 */
 void game_loop(SDL_Instance gInstance)
 {
	int action;
	while (true)
	{
		action = poll_events();
		if (action < 0)
			break;
		do_action(action, &(gInstance.player), gInstance.map);

		render_graphics(&gInstance);
	}
 }