#include "../inc/maze.h"


/**
 * render_graphics - Renders the maze graphics onto the screen
 * @gInstance: SDL game instance
 * Return: void
 */
void render_graphics(SDL_Instance *gInstance)
{
	int i;
	int pp_distance; /* Distance between player and project plane */
	RendColumn column;
	SDL_Renderer *gRenderer = gInstance->renderer;
	MazePlayer player = gInstance->player;

	pp_distance = (WINDOW_WIDTH / 2) / (tanf(to_radians(FIELD_OF_VIEW / 2)));

	/* Clear screen */
	/* SDL_SetRenderDrawColor(gRenderer, 0xEF, 0xB9, 0x6E, 0xFF); */
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0xAE, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);

	paintFloor(gRenderer); /* Default floor color */

	/* Render walls and floor */
	for (i = 0; i <= WINDOW_WIDTH; i++)
	{
		column.index = i;

		getWallDistance(&column, player, gInstance->map);
		/*printf("col: %d, wall distance: %d\n", i, wall_distance);*/
		drawWallSlice(&column, pp_distance, gInstance);

		if (gInstance->floor_texture.mTexture != NULL)
			drawFloorSlice(gInstance, column, player, pp_distance);
	}

	SDL_RenderPresent(gRenderer);
}

