#include "../inc/maze.h"

/**
 * init_texture - Initialises a texture struct
 * @texture: The texture struct to be initialised
 * Return: Nothing
 */
void init_texture(M_Texture *texture)
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
void freeTexture(M_Texture *texture)
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
 * loadTexture - Loads a texture with the the image from a file
 * @gRenderer: SDL renderer
 * @texture: The texture to be loaded
 * @filepath: The file path containing the texture image
 * Return: TRUE if successful. Otherwise return FALSE
 */
bool loadTexture(SDL_Renderer *gRenderer, M_Texture *texture,
char *filepath)
{

	SDL_Texture *newTexture;
	SDL_Surface *surface;

	freeTexture(texture);

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

	texture->width = surface->w;
	texture->height = surface->h;
	texture->mTexture = newTexture;

	SDL_FreeSurface(surface);

	return (true);
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