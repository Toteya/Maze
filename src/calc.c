#include "../inc/maze.h"

int fix_distortion(float distance, float view_angle, float ray_angle);
float getDistToHorizonalWall(MazePlayer, int map[][MAP_WIDTH],
int dir_x, int dir_y, float ray_angle);
float getDistToVerticalWall(MazePlayer, int map[][MAP_WIDTH],
int dir_x, int dir_y, float ray_angle);
float selectDistance(float distToHorWall, float distToVertWall,
int dir_x, int dir_y, RenderColumn *column);

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
 * @player: The game player
 * @map: The wall map array
 * Return: the distance between the ray and the wall
 */
void getWallDistance(RenderColumn *column, MazePlayer player,
int map[][MAP_WIDTH])
{
	float view_angle = player.view_angle;
	float angle_b_rays; /* Angle between subsequent rays*/
	float ray_angle;
	float distToVertWall = 0; /* distance to vertical wall intersect */
	float distToHorWall = 0; /* distance to horizontal wall intersect */
	float distance; /* Distance from player viewpoint to wall*/
	int dir_y, dir_x; /* Direction of ray in y and x axes*/

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

	distToHorWall = getDistToHorizonalWall(player, map, dir_x, dir_y, ray_angle);
	distToVertWall = getDistToVerticalWall(player, map, dir_x, dir_y, ray_angle);

	distance = selectDistance(distToHorWall, distToVertWall, dir_x, dir_y,
	column);

	column->distance = fix_distortion(distance, view_angle, ray_angle);
}

/**
 * selectDistance - Compares the distance to a horizontal wall intersection
 * with the distance to a vertical wall intersection and selects the shorter
 * one.
 * @distToHorWall: Distance from the player to a horizontal wall intersection.
 * @distToVertWall: Distance from the player to a vertical wall intersection.
 * @dir_x: The x-direction of the ray (LEFT or RIGHT)
 * @dir_y: The y-direction of the ray (UP or DOWN)
 * @column: The ray's column to be rendered.
 * Return: the selected distance
 */
float selectDistance(float distToHorWall, float distToVertWall,
int dir_x, int dir_y, RenderColumn *column)
{
	float distance;
	/* NOTE: potential bug - CASE: distance is correctly ZERO*/
	if (distToHorWall)
	{
		distance = fabs(distToHorWall);
		if (dir_y == Y_DIRECTION_UP)
			column->direction = MAZE_SOUTH;
		else
			column->direction = MAZE_NORTH;
	}
	else
	{
		distance = fabs(distToVertWall);
		if (dir_x == X_DIRECTION_LEFT)
			column->direction = MAZE_EAST;
		else
			column->direction = MAZE_WEST;
	}
	if (distToVertWall && fabs(distToVertWall) < fabs(distToHorWall))
	{
		distance = fabs(distToVertWall);
		if (dir_x == X_DIRECTION_LEFT)
			column->direction = MAZE_EAST;
		else
			column->direction = MAZE_WEST;
	}

	return (distance);
}

/**
 * getDistToHorizontalWall - Calculates the distance from the player to the
 * closest horizontal intersection of a ray and a wall for the given
 * ray angle / column
 * @player: The player
 * @map: The maze map
 * @dir_x: The x-direction of the ray (LEFT or RIGHT)
 * @dir_y: The y-direction of the ray (UP or DOWN)
 * @ray_angle: The ray's angle
 * Return: Distance to wall
 */
float getDistToHorizonalWall(MazePlayer player, int map[][MAP_WIDTH],
int dir_x, int dir_y, float ray_angle)
{
	float distanceToHorWall = 0;
	float A_x, A_y; /* x- and y- distances to grid intersection */
	int A_x_grid = 0, A_y_grid = 0; /* Grid coordinates */
	bool wall_found;

	if (dir_y == Y_DIRECTION_UP)
		A_y = (player.pos.y / GRID_INTERVAL) * GRID_INTERVAL - 1;
	else if (dir_y == Y_DIRECTION_DOWN)
		A_y = (player.pos.y / GRID_INTERVAL) * GRID_INTERVAL + GRID_INTERVAL;
	else
		A_y = player.pos.y; /* ray is purely horizontal - don't change y */
	A_x = player.pos.x + (player.pos.y - A_y) / tanf(to_radians(ray_angle));

	wall_found = false;
	while (dir_x && (A_x_grid >= 0 && A_x_grid < MAP_WIDTH) &&
	(A_y_grid >= 0 && A_y_grid < MAP_HEIGHT))
	{
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
			distanceToHorWall =  (player.pos.x - A_x) / cosf(to_radians(ray_angle));
		else
			distanceToHorWall = A_y - player.pos.y;
	}

	return (distanceToHorWall);
}

/**
 * getDistToVerticalWall - Calculates the distance from the player to the
 * closest vertical intersection of a ray and a wall for the given
 * ray angle / column
 * @player: The player
 * @map: The maze map
 * @dir_x: The x-direction of the ray
 * @dir_y: The y-direction of the ray
 * @ray_angle: The ray's angle
 * Return: Distance to wall
 */
float getDistToVerticalWall(MazePlayer player, int map[][MAP_WIDTH],
int dir_x, int dir_y, float ray_angle)
{
	float distanceToVertWall = 0;
	float A_x, A_y; /* x and y distances to grid intersection */
	int A_x_grid = 0, A_y_grid = 0; /* Grid coordinates */
	bool wall_found;

	if (dir_x == X_DIRECTION_LEFT)
		A_x = (player.pos.x / GRID_INTERVAL) * GRID_INTERVAL - 1;
	else if (dir_x == X_DIRECTION_RIGHT)
		A_x = (player.pos.x / GRID_INTERVAL) * GRID_INTERVAL + GRID_INTERVAL;
	else
		A_x = player.pos.x; /* The ray is purely vertical -> don't change x*/
	A_y = player.pos.y + (player.pos.x - A_x) * tanf(to_radians(ray_angle));

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
			distanceToVertWall = (player.pos.x - A_x) / cosf(to_radians(ray_angle));
		else
			distanceToVertWall = A_x - player.pos.x;
	}

	return (distanceToVertWall);
}

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
