#include "../inc/maze.h"

void draw_wall_slice(RenderColumn, int, SDL_Renderer *);
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
	RenderColumn column;
	SDL_Renderer *gRenderer = gInstance->renderer;
	MazePlayer player = gInstance->player;

	pp_distance = (WINDOW_WIDTH / 2) / (tanf(to_radians(FIELD_OF_VIEW / 2)));

	SDL_SetRenderDrawColor(gRenderer, 0xEF, 0xB9, 0x6E, 0xFF);
	SDL_RenderClear(gRenderer);

	for (i = 0; i <= WINDOW_WIDTH; i++)
	{
		column.index = i;
		getWallDistance(&column, player, gInstance->map);
		/*printf("col: %d, wall distance: %d\n", i, wall_distance);*/

		draw_wall_slice(column, pp_distance, gRenderer);
	}

	SDL_RenderPresent(gRenderer);
}



/**
 * draw_wall_slice - Draws a wall slice onto the projection plane
 * @pp_dist: The distance from the player to the projection plane
 * @c: The column to be rendered
 * @gRenderer: The SDL renderer
 * Return: Nothing
 */
void draw_wall_slice(RenderColumn c, int pp_dist, SDL_Renderer *gRenderer)
{
	int wall_height = GRID_INTERVAL; /* Actual wall height*/
	int proj_height; /* Wall projection height */
	int start_point, end_point;
	SDL_Color w_color;

	proj_height = wall_height * ((float) pp_dist / c.distance);
	/**
	* printf("col: %d, Dw: %d, Dp: %d, Hw: %d, Hp: %d\n",
	* column, wall_distance, pp_distance, wall_height, proj_height);
	*/

	start_point = (WINDOW_HEIGHT / 2) - (proj_height / 2);
	end_point = start_point + proj_height;

	set_wall_color(c.direction, &w_color);
	SDL_SetRenderDrawColor(gRenderer, w_color.r, w_color.g, w_color.g, 0xFF);
	SDL_RenderDrawLine(gRenderer, c.index, start_point, c.index, end_point);
}

/**
 * set_wall_color - Sets the wall shade based on the direction the wall
 * is facing
 * @direction: The direction that the wall is facing
 * @wall_color: The wall color to be updated
 * Return: Nothing
 */
void set_wall_color(int direction, SDL_Colour *wall_color)
{
	/**
	 * Natural grey: #8c867f ->
	 *
	 */
	switch (direction)
	{
		case NORTH:
			wall_color->r = 0x58;
			wall_color->g = 0x53;
			wall_color->b = 0x4B;
			wall_color->a = 0xFF;
			break;
		case SOUTH:
			wall_color->r = 0xAF;
			wall_color->g = 0xA5;
			wall_color->b = 0x96;
			wall_color->a = 0xFF;
			break;
		case EAST:
			wall_color->r = 0x7A;
			wall_color->g = 0x73;
			wall_color->b = 0x69;
			wall_color->a = 0xFF;
			break;
		case WEST:
			wall_color->r = 0x8C;
			wall_color->g = 0x84;
			wall_color->b = 0x78;
			wall_color->a = 0xFF;
			break;
		default:
			break;
	}
}
