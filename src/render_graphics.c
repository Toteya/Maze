#include "../inc/maze.h"

void drawWallSlice(RendColumn *, int, SDL_Instance *);
void set_wall_color(int direction, SDL_Colour *wall_color);

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



/**
 * drawWallSlice - Draws a wall slice onto the projection plane
 * @pp_dist: The distance from the player to the projection plane
 * @column: The column to be rendered
 * @gInstance: The maze game SDL instance
 * Return: Nothing
 */
void drawWallSlice(RendColumn *column, int pp_dist, SDL_Instance *gInstance)
{
	int wall_height = WALL_HEIGHT; /* Actual wall height*/
	int wp_height; /* Wall projection height */
	int y_start, y_end;
	int type = column->type;
	SDL_Color wall_color;
	SDL_Renderer *gRenderer = gInstance->renderer;

	wp_height = wall_height * ((float) pp_dist / column->distance);
	y_start = (WINDOW_HEIGHT / 2) - (wp_height / 2);
	y_end = y_start + wp_height;
	column->wb_row = y_end;

	if (gInstance->wall_texture[type].mTexture != NULL)
	{
		renderWallTexture(gInstance, column->index, y_start, wp_height, *column);
	}
	else
	{
		set_wall_color(column->direction, &wall_color);
		SDL_SetRenderDrawColor(gRenderer, wall_color.r, wall_color.g, wall_color.g,
		0xFF);
		SDL_RenderDrawLine(gRenderer, column->index, y_start, column->index,
		y_end);
	}
}

/**
 * set_wall_color - Sets the wall shade based on the direction the wall
 * is facing (if no wall texture is rendered)
 * @direction: The direction that the wall is facing
 * @wall_color: The wall color to be updated
 * Return: Nothing
 */
void set_wall_color(int direction, SDL_Colour *wall_color)
{
	/**
	 * Natural grey: #8c867f ->
	 * Cloudy: #9c9895 -> #797673
	 *
	 */
	switch (direction)
	{
		case NORTH:
			wall_color->r = 0x8A;
			wall_color->g = 0x87;
			wall_color->b = 0x84;
			wall_color->a = 0xFF;
			break;
		case SOUTH:
			wall_color->r = 0x8A;
			wall_color->g = 0x87;
			wall_color->b = 0x84;
			wall_color->a = 0xFF;
			break;
		case EAST:
			wall_color->r = 0x79;
			wall_color->g = 0x76;
			wall_color->b = 0x73;
			wall_color->a = 0xFF;
			break;
		case WEST:
			wall_color->r = 0x79;
			wall_color->g = 0x76;
			wall_color->b = 0x73;
			wall_color->a = 0xFF;
			break;
		default:
			break;
	}
}
