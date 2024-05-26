#include "../inc/maze.h"

/**
 * poll_events - Polls events and returns an action code corresponding
 * to the given event.
 *
 * Return: Action code
 */
bool poll_events(int actions[])
{
	int i = 0;
	SDL_Event event;
	/* SDL_KeyboardEvent key; */

	const Uint8 *currentKeyStates;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			/* return (ACTION_QUIT); */
			return (false);
		}
	}

	currentKeyStates = SDL_GetKeyboardState(NULL);
	if (currentKeyStates[SDL_SCANCODE_ESCAPE])
		return (false);
	if (currentKeyStates[SDL_SCANCODE_UP])
		actions[i++] = ACTION_MOVE_FORWARD;
	if (currentKeyStates[SDL_SCANCODE_DOWN])
		actions[i++] = ACTION_MOVE_BACKWARD;
	if (currentKeyStates[SDL_SCANCODE_LEFT])
		actions[i++] = ACTION_MOVE_LEFT;
	if (currentKeyStates[SDL_SCANCODE_RIGHT])
		actions[i++] = ACTION_MOVE_RIGHT;
	if (currentKeyStates[SDL_SCANCODE_A])
		actions[i++] = ACTION_TURN_LEFT;
	if (currentKeyStates[SDL_SCANCODE_D])
		actions[i++] = ACTION_TURN_RIGHT;
	

	/* if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) */
	/*
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
						return (ACTION_MOVE_FORWARD);
					case SDLK_DOWN:
						return (ACTION_MOVE_BACKWARD);
					case SDLK_LEFT:
						return (ACTION_MOVE_LEFT);
					case SDLK_RIGHT:
						return (ACTION_MOVE_RIGHT);
					case SDLK_a:
						return (ACTION_TURN_LEFT);
					case SDLK_d:
						return (ACTION_TURN_RIGHT);
					default:
						break;
				}
		}
	*/
	
	/* return (ACTION_NONE); */
	return (true);
}
