#include "../inc/maze.h"

/**
 * poll_events - Polls events and returns an action code corresponding
 * to the given event.
 * @actions: An array of polled actions
 * Return: Action code
 */
bool poll_events(int actions[])
{
	int i = 0;
	SDL_Event event;

	const Uint8 *currentKeyStates;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			/* return (ACTION_QUIT); */
			return (false);
		}
	}

	/* Allows for simultaneous key events */
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

	return (true);
}
