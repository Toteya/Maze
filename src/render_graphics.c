#include "../inc/maze.h"

int find_wall(int, float, map_location);
float to_radians(float angle_deg);
bool check_for_wall(int x, int y);

/**
 * render_graphics - Renders the maze graphics onto the screen
 *
 * Return: void
 */
void render_graphics(void)
{
	Maze_player player = {
		{192, 1792},
		90
	};
	int i;

	for (i = 0; i <= WINDOW_WIDTH; i++)
	{
		find_wall(i, player.view_angle, player.pos);
	}
}

/**
 * find_wall - Calculates the distance of a ray from the player viewpoint
 * to a wall
 * @column: The ray (or column) whose distance to the wall is to be found
 * @view_angle: The player's angle of view.
 * Return: the distance between the ray and the wall
 */
int find_wall(int column, float view_angle, map_location player_pos)
{
	float angle_btwn_rays = (float) FIELD_OF_VIEW / WINDOW_WIDTH;
	float ray_angle;
	float beta; /* distort. cor. factor - angle between ray and viewing angle*/
	float dist_vertical_int; /* distance to the vertical intersect. with wall*/
	float dist_horizontal_int; /* distance to the horiz. intersect. with wall*/
	float distance; /* Distance from player viewpoint to wall*/
	bool wall_found;
	int count = 0;
	int dir_y, dir_x; /* Direction of ray in y and x axes*/

	int A_x, A_y; /* Grid intersection coordination */
	int A_x_grid, A_y_grid; /* Grid coordinates */

	ray_angle = view_angle + (FIELD_OF_VIEW / 2) - (column * angle_btwn_rays);
	if (ray_angle >= 360)
		ray_angle -= 360;
	else if (ray_angle < 0)
		ray_angle += 360;

	/* Checkk ray direction on y-axis */
	if (ray_angle > 0 && ray_angle < 180)
		dir_y = Y_DIRECTION_UP;
	else if (ray_angle < 0 || ray_angle > 180)
		dir_y = Y_DIRECTION_DOWN;
	else
		dir_y = Y_DIRECTION_NONE; /* ray direction is purely horizontal */

	/* Checkk ray direction on x-axis */
	if (ray_angle > 90 && ray_angle < 270 )
		dir_x = X_DIRECTION_LEFT;
	else if (ray_angle < 90 || ray_angle > 270)
		dir_x = X_DIRECTION_RIGHT;
	else
		dir_x = X_DIRECTION_NONE; /* The ray is purely vertical */

	/* printf("column: %d, angle: %f\n", column, ray_angle); */

	/* find a horizontal intersection with a wall */
	/* y-coordinate*/
	if (dir_y == Y_DIRECTION_UP)
		A_y = (player_pos.y / GRID_WIDTH) * GRID_WIDTH - 1;            
	else if (dir_y == Y_DIRECTION_DOWN)
		A_y = (player_pos.y / GRID_WIDTH) * GRID_WIDTH + GRID_WIDTH;
	else
	{
		/* ray is purely horizontal -> keep A_y equal player_pos(y) */
		A_y = player_pos.y;
	}
	/* x-cordinate */
	A_x = player_pos.x + (player_pos.y - A_y) / tanf(to_radians(ray_angle));

	while (true)
	{
		/* Find grid coordinates */
		A_y_grid = A_y / GRID_WIDTH;
		A_x_grid = A_x / GRID_WIDTH;
		if (check_for_wall(A_x_grid, A_y_grid))
			break;

		A_x = A_x + (GRID_WIDTH / tanf(to_radians(ray_angle))) * dir_x;
		A_y = A_y + GRID_WIDTH * dir_y;
	}

	dist_vertical_int =  abs(player_pos.x - A_x) / cosf(to_radians(ray_angle));

	/* find a vertical intersection with a wall */
	/* x-coordinate */
	if (dir_x == X_DIRECTION_LEFT)
		A_x = (player_pos.x / GRID_WIDTH) * GRID_WIDTH - 1;
	else if (dir_x == X_DIRECTION_RIGHT)
		A_x = (player_pos.x / GRID_WIDTH) * GRID_WIDTH + GRID_WIDTH;
	else
		A_x = player_pos.x; /* The ray is purely vertical -> don't change */
	/* y-coordinate*/
	A_y = player_pos.y + (player_pos.x - A_x) * tanf(to_radians(ray_angle));

	while (true)
	{
		A_x_grid = A_x / GRID_WIDTH;
		A_y_grid = A_y / GRID_WIDTH;
		if (check_for_wall(A_x_grid, A_y_grid))
			break;
		
		A_x = A_x + (GRID_WIDTH * dir_x);
		A_y = A_y + (GRID_WIDTH * tanf(to_radians(ray_angle)));
	}

	dist_horizontal_int =  abs(player_pos.x - A_x) / cosf(to_radians(ray_angle));

	if (dist_horizontal_int < dist_vertical_int)
		distance = dist_horizontal_int;
	else
		distance = dist_vertical_int;
	
	/* Distortion correction */
	beta = abs(ray_angle - view_angle);
	if (beta > 180)
		beta = 360 - beta;

	distance = distance * cosf(to_radians(beta));

	return (distance);

}

/**
 * check_for_wall - Checks if there is a wall at the given grid position
 * @x: The x coordinate of the grid position to be checked.
 * @y: The y coordinate of the grid position to be checked
 * Return: (bool) TRUE if there is a wall. Otherwise return FALSE.
 */
bool check_for_wall(int x, int y)
{
	/* to be changed later */
	if (x && y)
		return true;
}

/**
 * to_radians - Converts degrees to radians
 * @angle_deg: The angle in degrees to be converted to radians
 * Return: The angle in radians
 */
float to_radians(float angle_deg)
{
	float angle_rad = angle_deg * (PI / 180);

	return angle_rad;
}
