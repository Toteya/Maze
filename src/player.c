#include "../inc/maze.h"

/**
 * init_player - Initialises the player
 * @player: Pointer to player instance to be intialised
 *
 * Return: Nothing.
 */
void init_player(MazePlayer *player)
{
	MazePlayer p = {
		{192, 1792},
		45
	};

	*player = p;
}
