#include "../inc/maze.h"

/**
 * do_action - Performs an action according to the given action code
 * @actions: An array of polled action codes specifying the action to performed
 * @player: The player
 * @map: The maze map
 * Return: Nothing
 */
void do_action(int actions[], MazePlayer *player, int map[][MAP_WIDTH])
{
	int i = 0;

	while (actions[i])
	{
		switch (actions[i])
		{
			case ACTION_TURN_LEFT:
				do_turn(actions[i], player);
				break;
			case ACTION_TURN_RIGHT:
				do_turn(actions[i], player);
				break;
			case ACTION_MOVE_FORWARD:
				do_move(actions[i], player, map);
				break;
			case ACTION_MOVE_BACKWARD:
				do_move(actions[i], player, map);
				break;
			case ACTION_MOVE_LEFT:
				do_move(actions[i], player, map);
				break;
			case ACTION_MOVE_RIGHT:
				do_move(actions[i], player, map);
				break;
			default:
				break;
		}
		i++;
	}
}

/**
 * clear_actions - initaliases the actions array to zero
 * @actions: The array of event polled actions
 * Return: Nothing.
 */
void clear_actions(int actions[])
{
	int i;

	for (i = 0; i < MAX_ACTIONS; i++)
		actions[i] = 0;
}
