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
		{PLAYER_START_POS_X, PLAYER_START_POS_Y},
		PLAYER_START_VIEW_ANGLE
	};

	*player = p;
}
