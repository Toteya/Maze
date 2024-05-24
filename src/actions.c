#include "../inc/maze.h"

/**
 * do_action - Performs an action according to the given action code
 * @action: The code specifying the action to performed
 * @player: The player
 * Return: Nothing
 */
void do_action(int action, MazePlayer *player, int map[][MAP_WIDTH])
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
			do_move(action, player, map);
			break;
		case ACTION_BACKWARD:
			do_move(action, player, map);
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

	if (player->view_angle > 360)
		player->view_angle -= 360;
	else if (player->view_angle < 0)
		player->view_angle += 360;
}

/**
 * do_move - Moves the player forward or backward based on the given
 * direction of movement
 * @action: The action code specifying the direcion
 * @player: The player
 * @map: The maze map
 * Return: Nothing
 */
void do_move(int action, MazePlayer *player, int map[][MAP_WIDTH])
{
	float angle_rad;
	int new_x, new_y;
	int A_x_grid = 0, A_y_grid = 0; /* Grid coordinates */

	if (action == ACTION_FORWARD)
		angle_rad = to_radians(player->view_angle);
	else
		angle_rad = to_radians(player->view_angle + 180);

	/* printf("deg: %f, rad: %f\n", player->view_angle, angle_rad); */

	new_x = player->pos.x + ((float) MOVE_STEP * cosf(angle_rad));
	new_y = player->pos.y - ((float) MOVE_STEP * sinf(angle_rad));

	A_x_grid = new_x / GRID_INTERVAL;
	A_y_grid = new_y / GRID_INTERVAL;
	
	if (!check_for_wall(A_x_grid, A_y_grid, map))
	{
		player->pos.x = new_x;
		player->pos.y = new_y;
	}
	printf("Pos: [%d, %d]\n", player->pos.x, player->pos.y);
}
