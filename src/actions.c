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
			do_move(action, player);
			break;
		case ACTION_BACKWARD:
			do_move(action, player);
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
 * do_move - Moves the player forward or backward based on the given
 * direction of movement
 * @action: The action code specifying the direcion
 * @player: The player
 * Return: Nothing
 */
void do_move(int action, MazePlayer *player)
{
	float angle_rad;

	if (action == ACTION_FORWARD)
		angle_rad = to_radians(player->view_angle);
	else
		angle_rad = to_radians(player->view_angle + 180);

	printf("deg: %f, rad: %f\n", player->view_angle, angle_rad);

	player->pos.x = player->pos.x + ((float) MOVE_STEP * cosf(angle_rad));
	player->pos.y = player->pos.y - ((float) MOVE_STEP * sinf(angle_rad));

	/* printf("Pos: [%d, %d]\n", player->pos.x, player->pos.y); */
}
