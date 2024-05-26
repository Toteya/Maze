#include "../inc/maze.h"

/**
 * init_Texture - Initialises a texture struct
 * @texture: The texture struct to be initialised
 * Return: Nothing
 */
void init_Texture(WTexture *texture)
{
	texture->mTexture = NULL;
	texture->width = 0;
	texture->height = 0;
}

/**
 * freeTexture - Frees/clears a texture
 * @texture: The texture to be freed
 * Return: Nothing.
 */
void freeTexture(WTexture *texture)
{
	if (texture->mTexture != NULL)
	{
		SDL_DestroyTexture(texture->mTexture);
		texture->mTexture = NULL;
		texture->width = 0;
		texture->height = 0;
	}
}

/**
 * loadTextureFromFile - Loads a texture with the the image from a file
 * @gInstance: The maze game SDL instance
 * @filepath: The file path containing the texture image
 * Return: TRUE if successful. Otherwise return FALSE
 */
bool loadTextureFromFile(SDL_Instance *gInstance, char *filepath)
{

	SDL_Renderer *gRenderer = gInstance->renderer;
	WTexture *gTexture = &(gInstance->texture);
	SDL_Texture *newTexture;
	SDL_Surface *surface;

	freeTexture(gTexture);

	surface = IMG_Load(filepath);
	if (surface == NULL)
	{
		fprintf(stderr, "Could not load image %s! SDL_image Error: %s\n",
		filepath, IMG_GetError());
		return (false);
	}

	newTexture = SDL_CreateTextureFromSurface(gRenderer, surface);
	if (newTexture == NULL)
	{
		fprintf(stderr, "Unable to create to texture from %s! SDL Error: %s\n",
		filepath, SDL_GetError());
		return (false);
	}

	gTexture->width = surface->w;
	gTexture->height = surface->h;
	gTexture->mTexture = newTexture;

	SDL_FreeSurface(surface);

	return (true);
}

/**
 * renderTexture - Renders a wall texture onto the screen
 * @gInstance: The maze game SDL instance
 * @x: The x position on the screen to render
 * @y: The y position on the screen to render
 * @wp_height: The projected wall height
 * @wallPos: The ray's position on the wall block
 */
void renderTexture(SDL_Instance *gInstance, int x, int y, int wp_height,
int wallPos)
{
	SDL_Renderer *gRenderer = gInstance->renderer;
	WTexture *gTexture = &(gInstance->texture);

	SDL_Rect renderRect; /* Destination rectangle on screen to render*/
	SDL_Rect slice; /* The source texture slice/clip */
	int texturePos;

	texturePos = gTexture->width * ((float)wallPos / GRID_INTERVAL);
	slice.x = texturePos;
	slice.y = 0;
	slice.w = 1;
	slice.h = gTexture->height;

	renderRect.x = x;
	renderRect.y = y;
	renderRect.w = slice.w;
	renderRect.h = wp_height;

	SDL_RenderCopy(gRenderer, gTexture->mTexture, &slice, &renderRect);
}
