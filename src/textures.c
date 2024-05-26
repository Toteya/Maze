#include "../inc/maze.h"

void initTexture(WTexture *texture)
{
	texture->mTexture = NULL;
	texture->width = 0;
	texture->height = 0;
}

void freeTexture(WTexture *texture)
{
	if (texture->mTexture != NULL)
	{
		SDL_DestroyTexture(texture->mTexture);
		texture->width = 0;
		texture->height = 0;
	}
}

bool loadTextureFromFile(SDL_Instance *gInstance, char *filepath)
{

	SDL_Renderer *gRenderer = gInstance->renderer;
	WTexture *gTexture = &(gInstance->texture);
	SDL_Texture *newTexture;
	SDL_Surface *surface;

	freeTexture(gTexture);

	surface = IMG_Load(filepath);
	if(surface == NULL)
	{
		fprintf(stderr, "Could not load image %s! SDL_image Error: %s\n", filepath, IMG_GetError());
		return (false);
	}

	newTexture = SDL_CreateTextureFromSurface(gRenderer, surface);
	if (newTexture == NULL)
	{
		fprintf(stderr, "Unable to create to texture from %s! SDL Error: %s\n", filepath, SDL_GetError());
		return (false);
	}

	gTexture->width = surface->w;
	gTexture->height = surface->h;

	SDL_FreeSurface(surface);

	gTexture->mTexture = newTexture;
	return (true);
}

void renderTexture(SDL_Instance *gInstance, int x, int y, int wp_height, int wallPos)
{
	SDL_Renderer *gRenderer = gInstance->renderer;
	WTexture *gTexture = &(gInstance->texture);
	
	SDL_Rect renderQuad;
	SDL_Rect slice;
	int texturePos;

	texturePos = gTexture->width * ((float)wallPos / GRID_INTERVAL);
	slice.x = texturePos;
	slice.y = 0;
	slice.w = 1;
	slice.h = gTexture->height;

/*
	printf("wallPos = %d\n", wallPos);
	printf("slice.x = %d\n", texturePos);
	printf("slice.y = %d\n", 0);
	printf("slice.w = %d\n", 1);
	printf("slice.h = %d\n", gTexture->height);
*/
	renderQuad.x = x;
	renderQuad.y = y;
	renderQuad.w = slice.w;
	renderQuad.h = wp_height;

	SDL_RenderCopy(gRenderer, gTexture->mTexture, &slice, &renderQuad);
}