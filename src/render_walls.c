#include "../inc/maze.h"

void shadeTexture(M_Texture *texture, int direction);
void set_wall_color(int direction, SDL_Colour *wall_color);


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

/**
 * renderWallTexture - Renders a wall texture slice onto the screen
 * @gInstance: The maze game SDL instance
 * @x: The x position on the screen to render
 * @y: The y position on the screen to render
 * @wp_height: The projected wall height
 * @column: The column to be rendered
 */
void renderWallTexture(SDL_Instance *gInstance, int x, int y, int wp_height,
RendColumn column)
{
	SDL_Renderer *gRenderer = gInstance->renderer;
	M_Texture *gTexture = &(gInstance->wall_texture[column.type]);

	SDL_Rect renderRect; /* Destination rectangle on screen to render*/
	SDL_Rect slice; /* The source texture slice/clip */
	int texturePos;
	int wallPos = column.wall_pos;
	int direction = column.direction;

	texturePos = gTexture->width * ((float)wallPos / GRID_INTERVAL) - 1;

	slice.x = texturePos;
	slice.y = 0;
	slice.w = 1;
	slice.h = gTexture->height;

	renderRect.x = x;
	renderRect.y = y;
	renderRect.w = slice.w;
	renderRect.h = wp_height;

	shadeTexture(gTexture, direction);
	SDL_RenderCopy(gRenderer, gTexture->mTexture, &slice, &renderRect);
}

/**
 * shadeTexture - Applies shading to a wall Texture, depending on the direction
 * it is facing
 * @texture: The texture to be shaded.
 * @direction: The direction the wall is facing.
 * Return: Nothing
 */
void shadeTexture(M_Texture *texture, int direction)
{
	float shading_factor = 1;
	Uint8 r;
	Uint8 g;
	Uint8 b;

	if (direction == EAST || direction == WEST)
		shading_factor = 0.75;

	r = 0xFF * shading_factor;
	g = 0xFF * shading_factor;
	b = 0xFF * shading_factor;

	SDL_SetTextureColorMod(texture->mTexture, r, g, b);
}
