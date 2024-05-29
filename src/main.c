#include <SDL2/SDL.h>
#include "../inc/maze.h"


void game_loop(SDL_Instance gInstance);
void init_wallTextureFilepaths(char *files[]);

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
	char *floor_texture_filepath = "images/floor_02.png$$";
	char *wall_texture_filepath[TOTAL_WALL_TYPES];
	int i;

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

	/* Initialise and load wall textures */
	init_wallTextureFilepaths(wall_texture_filepath);
	for (i = 0; i < TOTAL_WALL_TYPES; i++)
	{
		init_texture(&(gInstance.wall_texture[i]));

		if (!loadTexture(gInstance.renderer, &(gInstance.wall_texture[i]),
			wall_texture_filepath[i]))
			fprintf(stderr, "Failed to load wall texture from file.\n");
	}

	/* Initialise and load floor texture */
	init_texture(&(gInstance.floor_texture));

	if (!loadTexture(gInstance.renderer, &(gInstance.floor_texture),
		floor_texture_filepath))
		fprintf(stderr, "Failed to load floor texture from file.\n");

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

/**
 * init_wallTextureFilepaths - Initialises an array of filepaths containing
 * wall textures
 * @files: An array of filepaths
 */
void init_wallTextureFilepaths(char *files[])
{
	int i;

	for (i = 0; i < TOTAL_WALL_TYPES; i++)
	{
		switch (i)
		{
		case DEFAULT_WALL:
			files[i] = "images/wall_default.png";
			break;
		case START_1:
			files[i] = "images/wall_start_01.png";
			break;
		case START_2:
			files[i] = "images/wall_start_02.png";
			break;
		case FINISH_1:
			files[i] = "images/wall_finish_01.png";
			break;
		case FINISH_2:
			files[i] = "images/wall_finish_02.png";
			break;
		default:
			break;
		}
	}
}
