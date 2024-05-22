#include "../inc/maze.h"

/**
 * do_action - Performs an action according to the given action code
 * @action: The code specifying the action to performed
 * @player: The player
 * Return: Nothing
 */
void do_action(int action, MazePlayer *player)
{
	switch (action)
	{
		case ACTION_LEFT:
			do_turn(action, player);
			break;
		case ACTION_RIGHT:
			do_turn(action, player);
			break;
		case ACTION_FORWARD:
			do_forward(player);
			break;
		default:
			break;
	}
}

/**
 * do_turn - Rotates the player's viewpoint according to the given direction
 * of rotation
 * @action: the action code specifying the direction of rotation
 * @player: the player
 * Return: Nothing
 */
void do_turn(int action, MazePlayer *player)
{
	if (action == ACTION_LEFT)
		player->view_angle++;
	else
		player->view_angle--;
}

/**
 * do_forward - Moves the player forward
 * @player: The player
 * Return: Nothing
 */
void do_forward(MazePlayer *player)
{
	float angle_rad;

	angle_rad = to_radians(player->view_angle);
	player->pos.x = player->pos.x + ((float) MOVE_STEP * cosf(angle_rad));
	player->pos.y = player->pos.y - ((float) MOVE_STEP * sinf(angle_rad));

	/* printf("Pos: [%d, %d]\n", player->pos.x, player->pos.y); */
}
