#include "../inc/maze.h"

/**
 * poll_events - Polls events and returns an action code corresponding
 * to the given event.
 *
 * Return: Action code
 */
int poll_events(void)
{
	SDL_Event event;
	SDL_KeyboardEvent key;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				return (ACTION_QUIT);
			case SDL_KEYDOWN:
				key = event.key;
				switch (key.keysym.sym)
				{
					case SDLK_ESCAPE:
						return (ACTION_QUIT);
					case SDLK_UP:
						return (ACTION_FORWARD);
					case SDLK_DOWN:
						return (ACTION_BACKWARD);
					case SDLK_LEFT:
						return (ACTION_LEFT);
					case SDLK_RIGHT:
						return (ACTION_RIGHT);
					default:
						break;
				}
		}
	}
	return (ACTION_NONE);
}
