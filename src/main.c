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

	/* Initialise game instance */
	if (init_instance(&gInstance) == false)
	{
		return (EXIT_FAILURE);
	}

	if (argc > 1)
		map_filename = argv[1];
	if (init_map(gInstance.map, map_filename) == false)
	{
		fprintf(stderr, "Map failed to initialise\n");
		return (EXIT_FAILURE);
	}

	init_player(&(gInstance.player));

	init_texture(&(gInstance.wall_texture));
	init_texture(&(gInstance.floor_texture));

	/* Load media */
	if (!loadTexture(gInstance.renderer, &(gInstance.wall_texture),
		"images/wall_texture_02.png"))
	{
		fprintf(stderr, "Failed to load wall texture from file.\n");
	}
	if (!loadTexture(gInstance.renderer, &(gInstance.floor_texture),
		"images/floor_texture_02.png"))
	{
		fprintf(stderr, "Failed to load floor texture from file.\n");
	}

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
	int actions[MAX_ACTIONS];

	while (true)
	{
		clear_actions(actions);
		/* if (actions[0] == ACTION_QUIT) */
		if (!(poll_events(actions)))
			break;

		do_action(actions, &(gInstance.player), gInstance.map);
		render_graphics(&gInstance);
	}
}
