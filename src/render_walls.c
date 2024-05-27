#include "../inc/maze.h"

void shadeTexture(M_Texture *texture, int direction);

/**
 * renderWallTexture - Renders a wall texture slice onto the screen
 * @gInstance: The maze game SDL instance
 * @x: The x position on the screen to render
 * @y: The y position on the screen to render
 * @wp_height: The projected wall height
 * @column: The column to be rendered
 * @wallPos: The ray's position on the wall block
 */
void renderWallTexture(SDL_Instance *gInstance, int x, int y, int wp_height,
RendColumn column)
{
	SDL_Renderer *gRenderer = gInstance->renderer;
	M_Texture *gTexture = &(gInstance->wall_texture);

	SDL_Rect renderRect; /* Destination rectangle on screen to render*/
	SDL_Rect slice; /* The source texture slice/clip */
	int texturePos;
	int wallPos = column.wall_pos;
	int direction = column.direction;

	texturePos = gTexture->width * ((float)wallPos / GRID_INTERVAL);
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
