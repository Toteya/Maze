#include "../inc/maze.h"

int fix_distortion(float distance, float view_angle, float ray_angle);

/**
 * to_radians - Converts degrees to radians
 * @angle_deg: The angle in degrees to be converted to radians
 * Return: The angle in radians
 */
float to_radians(float angle_deg)
{
	float angle_rad = angle_deg * (PI / 180);

	return (angle_rad);
}

/**
 * getWallDistance - Calculates the distance from the player's viewpoint
 * to a wall
 * @column: The ray (or column) whose distance to the wall is to be found
 * @p: The game player
 * @map: The wall map array
 * Return: the distance between the ray and the wall
 */
void getWallDistance(RenderColumn *column, MazePlayer p, int map[][MAP_WIDTH])
{
	map_coordinate player_pos = p.pos;
	float view_angle = p.view_angle;
	float angle_b_rays; /* Angle between subsequent rays*/
	float ray_angle;
	float dist_ver_int = 0; /* distance to vertical wall intersect */
	float dist_hor_int = 0; /* distance to horizontal wall intersect */
	float distance; /* Distance from player viewpoint to wall*/
	int dir_y, dir_x; /* Direction of ray in y and x axes*/
	float A_x, A_y; /* Grid intersection coordination */
	int A_x_grid = 0, A_y_grid = 0; /* Grid coordinates */
	bool wall_found;

	angle_b_rays = (float) FIELD_OF_VIEW / WINDOW_WIDTH;
	ray_angle = view_angle + (FIELD_OF_VIEW / 2) - (column->index * angle_b_rays);
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

	/* Check ray direction on x-axis */
	if (ray_angle > 90 && ray_angle < 270)
		dir_x = X_DIRECTION_LEFT;
	else if (ray_angle < 90 || ray_angle > 270)
		dir_x = X_DIRECTION_RIGHT;
	else
		dir_x = X_DIRECTION_NONE; /* The ray is purely vertical */

	/* find a horizontal intersection with a wall */
	if (dir_y == Y_DIRECTION_UP)
		A_y = (player_pos.y / GRID_INTERVAL) * GRID_INTERVAL - 1;
	else if (dir_y == Y_DIRECTION_DOWN)
		A_y = (player_pos.y / GRID_INTERVAL) * GRID_INTERVAL + GRID_INTERVAL;
	else
	{
		/* ray is purely horizontal -> keep A_y equal player_pos(y) */
		A_y = player_pos.y;
	}
	A_x = player_pos.x + (player_pos.y - A_y) / tanf(to_radians(ray_angle));

	wall_found = false;
	while (dir_x && (A_x_grid >= 0 && A_x_grid < MAP_WIDTH) &&
	(A_y_grid >= 0 && A_y_grid < MAP_HEIGHT))
	/*while (A_x_grid >= 0 && A_y_grid >= 0)*/
	{
		/* Find grid coordinates */
		A_y_grid = A_y / GRID_INTERVAL;
		A_x_grid = A_x / GRID_INTERVAL;
		/**
		* printf("col: %d, Angle: %f - Hor: Grid [%d, %d] Pos[%f, %f]\n",
		* column->index, ray_angle, A_x_grid, A_y_grid, A_x, A_y);
		*/
		wall_found = check_for_wall(A_x_grid, A_y_grid, map);
		if (wall_found)
			break;
		A_y_grid = (A_y - dir_y) / GRID_INTERVAL;
		wall_found = check_for_wall(A_x_grid, A_y_grid, map);
		if (wall_found)
			break;

		A_y = A_y + GRID_INTERVAL * dir_y;
		if (dir_x)
			A_x = A_x + fabs(GRID_INTERVAL / tanf(to_radians(ray_angle))) * dir_x;
	}

	if (wall_found)
	{
		if (dir_x)
			dist_hor_int =  (player_pos.x - A_x) / cosf(to_radians(ray_angle));
		else
			dist_hor_int = A_y - player_pos.y;
	}

	/* find a vertical intersection with a wall */
	if (dir_x == X_DIRECTION_LEFT)
		A_x = (player_pos.x / GRID_INTERVAL) * GRID_INTERVAL - 1;
	else if (dir_x == X_DIRECTION_RIGHT)
		A_x = (player_pos.x / GRID_INTERVAL) * GRID_INTERVAL + GRID_INTERVAL;
	else
		A_x = player_pos.x; /* The ray is purely vertical -> don't change x*/
	A_y = player_pos.y + (player_pos.x - A_x) * tanf(to_radians(ray_angle));

	A_y_grid = 0;
	A_x_grid = 0;
	wall_found = false;
	while (dir_y && (A_x_grid >= 0 && A_x_grid < MAP_WIDTH) &&
	(A_y_grid >= 0 && A_y_grid < MAP_HEIGHT))
	{
		A_x_grid = A_x / GRID_INTERVAL;
		A_y_grid = A_y / GRID_INTERVAL;
		/**
		* printf("col: %d, Angle: %f - Ver: Grid [%d, %d] Pos[%f, %f]\n",
		* column->index, ray_angle, A_x_grid, A_y_grid, A_x, A_y);
		*/
		wall_found = check_for_wall(A_x_grid, A_y_grid, map);
		if (wall_found)
			break;
		A_x_grid = (A_x - dir_x) / GRID_INTERVAL;
		wall_found = check_for_wall(A_x_grid, A_y_grid, map);
		if (wall_found)
			break;

		A_x = A_x + (GRID_INTERVAL * dir_x);
		if (dir_y)
			A_y = A_y + fabs(GRID_INTERVAL * tanf(to_radians(ray_angle))) * dir_y;
	}

	if (wall_found)
	{
		if (dir_y)
			dist_ver_int = (player_pos.x - A_x) / cosf(to_radians(ray_angle));
		else
			dist_ver_int = A_x - player_pos.x;
	}

	/* NOTE to address potential bug - CASE: distance is correctly ZERO*/
	if (dist_hor_int)
	{
		distance = fabs(dist_hor_int);
		if (dir_y == Y_DIRECTION_UP)
			column->direction = SOUTH;
		else
			column->direction = NORTH;
	}
	else
	{
		distance = fabs(dist_ver_int);
		if (dir_x == X_DIRECTION_LEFT)
			column->direction = EAST;
		else
			column->direction = WEST;
	}
	if (dist_ver_int && fabs(dist_ver_int) < fabs(dist_hor_int))
	{
		distance = fabs(dist_ver_int);
		if (dir_x == X_DIRECTION_LEFT)
			column->direction = EAST;
		else
			column->direction = WEST;
	}

	column->distance = fix_distortion(distance, view_angle, ray_angle);
	/**
	* printf("col: %d, d_hor: %f, d_ver: %f, d: %f, d_cor: %d, dir: %d\n",
	* column->index, dist_hor_int, dist_ver_int, distance,
	* column->distance, column->direction);
	*/
}

/*
int getDistanceToVerticalWall()

int getDistanceToHorizontalWall()
*/


/**
 * fix_distortion - Corrects the given distance for distortion
 * @distance: The distance to be corrected (between the viewpoint and the wall)
 * @view_angle: The player's viewing angle
 * @ray_angle: The angle of the ray
 * Return: The corrected distance
 */
int fix_distortion(float distance, float view_angle, float ray_angle)
{
	float beta; /* Distortion cor. factor: Angle between ray and view angle */
	int corrected_distance;

	beta = fabs(ray_angle - view_angle);
	if (beta > 180)
		beta = 360 - beta;

	corrected_distance = distance * cosf(to_radians(beta));

	return (corrected_distance);
}


