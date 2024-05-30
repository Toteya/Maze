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
 * do_turn - Rotates the player's viewpoint according to the given direction
 * of rotation
 * @action: the action code specifying the direction of rotation
 * @player: the player
 * Return: Nothing
 */
void do_turn(int action, MazePlayer *player)
{
	if (action == ACTION_TURN_LEFT)
		player->view_angle += ROTATION_STEP;
	else
		player->view_angle -= ROTATION_STEP;

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

	if (action == ACTION_MOVE_FORWARD || action == ACTION_MOVE_RIGHT)
		angle_rad = to_radians(player->view_angle);
	else
		angle_rad = to_radians(player->view_angle + 180);

	if (action == ACTION_MOVE_FORWARD || action == ACTION_MOVE_BACKWARD)
	{
		new_x = roundf(player->pos.x + ((float) MOVE_STEP * cosf(angle_rad)));
		new_y = roundf(player->pos.y - ((float) MOVE_STEP * sinf(angle_rad)));
	}
	else
	{
		new_x = roundf(player->pos.x + ((float) MOVE_STEP * sinf(angle_rad)));
		new_y = roundf(player->pos.y + ((float) MOVE_STEP * cosf(angle_rad)));
	}

	A_x_grid = new_x / GRID_INTERVAL;
	A_y_grid = new_y / GRID_INTERVAL;

	if (!check_for_wall(A_x_grid, A_y_grid, map))
	{
		player->pos.x = new_x;
		player->pos.y = new_y;
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
