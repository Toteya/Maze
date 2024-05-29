#include "../inc/maze.h"

float getDistToHorizonalWall(MazePlayer, int map[][MAP_WIDTH],
	int dir_x, int dir_y, float ray_angle, int *wallPos, int *wallType);
float getDistToVerticalWall(MazePlayer, int map[][MAP_WIDTH],
	int dir_x, int dir_y, float ray_angle, int *wallPos, int *wallType);
float selectDistance(float distToHorWall, float distToVertWall,
	int dir_x, int dir_y, RendColumn *column, int wallPosHor, int wallPosVer,
	int wallTypeHor, int wallTypeVer);
void set_ray_direction(float ray_angle, int *dir_x, int *dir_y);


/**
 * getWallDistance - Calculates the distance from the player's viewpoint
 * to a wall
 * @column: The ray (or column) whose distance to the wall is to be found
 * @player: The game player
 * @map: The wall map array
 * Return: the distance between the ray and the wall
 */
void getWallDistance(RendColumn *column, MazePlayer player,
int map[][MAP_WIDTH])
{
	float view_angle = player.view_angle;
	float angle_b_rays; /* Angle between subsequent rays*/
	float ray_angle;
	float distToVertWall = 0; /* distance to vertical wall intersect */
	float distToHorWall = 0; /* distance to horizontal wall intersect */
	int wallTypeHor = DEFAULT_WALL;
	int wallTypeVer = DEFAULT_WALL;
	int wallPosHor = 0;
	int wallPosVer = 0;
	float distance; /* Distance from player viewpoint to wall*/
	int dir_y, dir_x; /* Direction of ray in y and x axes*/

	angle_b_rays = (float) FIELD_OF_VIEW / WINDOW_WIDTH;
	ray_angle = view_angle + (FIELD_OF_VIEW / 2) - (column->index * angle_b_rays);
	if (ray_angle >= 360)
		ray_angle -= 360;
	else if (ray_angle < 0)
		ray_angle += 360;
	column->ray_angle = ray_angle;
	/* column->type = DEFAULT_WALL; */

	set_ray_direction(ray_angle, &dir_x, &dir_y);


	distToHorWall = getDistToHorizonalWall(player, map, dir_x, dir_y,
	ray_angle, &wallPosHor, &wallTypeHor);
	distToVertWall = getDistToVerticalWall(player, map, dir_x, dir_y,
	ray_angle, &wallPosVer, &wallTypeVer);

	distance = selectDistance(distToHorWall, distToVertWall, dir_x, dir_y,
	column, wallPosHor, wallPosVer, wallTypeHor, wallTypeVer);

	column->distance = fix_distortion(distance, view_angle, ray_angle);
}

/**
 * set_ray_direction - Sets the ray's direction with respect to the x and y
 * axes
 * @ray_angle: The ray's angle
 * @dir_x: The x-direction of the ray (LEFT or RIGHT)
 * @dir_y: The y-direction of the ray (UP or DOWN)
 */
void set_ray_direction(float ray_angle, int *dir_x, int *dir_y)
{
	/* Check ray direction on y-axis */
	if (ray_angle > 0 && ray_angle < 180)
		*dir_y = Y_DIRECTION_UP;
	else if (ray_angle < 0 || ray_angle > 180)
		*dir_y = Y_DIRECTION_DOWN;
	else
		*dir_y = Y_DIRECTION_NONE; /* ray direction is purely horizontal */

	/* Check ray direction on x-axis */
	if (ray_angle > 90 && ray_angle < 270)
		*dir_x = X_DIRECTION_LEFT;
	else if (ray_angle < 90 || ray_angle > 270)
		*dir_x = X_DIRECTION_RIGHT;
	else
		*dir_x = X_DIRECTION_NONE; /* The ray is purely vertical */
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
 * @wallPosHor: The ray's position on the horizontal wall
 * @wallPosVer: The ray's position on the vertical wall
 * @wallTypeHor: The horizontal wall's type
 * @wallTypeVer: The vertical wall's type
 * Return: the selected distance
 */
float selectDistance(float distToHorWall, float distToVertWall,
	int dir_x, int dir_y, RendColumn *column, int wallPosHor, int wallPosVer,
	int wallTypeHor, int wallTypeVer)
{
	float distance;
	/* NOTE: potential bug - CASE: distance is correctly ZERO*/
	if (distToHorWall)
	{
		distance = fabs(distToHorWall);
		column->wall_pos = wallPosHor;
		column->type = wallTypeHor;
		if (dir_y == Y_DIRECTION_DOWN)
			column->direction = NORTH;
		else
		{
			column->direction = SOUTH;
			column->wall_pos = GRID_INTERVAL - column->wall_pos + 1;
		}
	}
	else
	{
		distance = fabs(distToVertWall);
		column->wall_pos = wallPosVer;
		column->type = wallTypeVer;
		if (dir_x == X_DIRECTION_LEFT)
			column->direction = EAST;
		else
		{
			column->direction = WEST;
			column->wall_pos = GRID_INTERVAL - column->wall_pos + 1;
		}
	}
	if (distToVertWall && fabs(distToVertWall) < fabs(distToHorWall))
	{
		distance = fabs(distToVertWall);
		column->wall_pos = wallPosVer;
		column->type = wallTypeVer;
		if (dir_x == X_DIRECTION_LEFT)
			column->direction = EAST;
		else
			column->direction = WEST;
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
 * @wallPos: The position of the ray on the wall block
 * @wallType: The type of wall
 * Return: Distance to wall
 */
float getDistToHorizonalWall(MazePlayer player, int map[][MAP_WIDTH],
int dir_x, int dir_y, float ray_angle, int *wallPos, int *wallType)
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
		*wallType = map[A_y_grid][A_x_grid] - 1;
		if (dir_x)
			distanceToHorWall =  (player.pos.x - A_x) / cosf(to_radians(ray_angle));
		else
			distanceToHorWall = A_y - player.pos.y;
		*wallPos = GRID_INTERVAL - (int)A_x % GRID_INTERVAL;
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
 * @wallPos: The position of the ray on the wall block
 * @wallType: The type of wall
 * Return: Distance to wall
 */
float getDistToVerticalWall(MazePlayer player, int map[][MAP_WIDTH],
int dir_x, int dir_y, float ray_angle, int *wallPos, int *wallType)
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
		*wallType = map[A_y_grid][A_x_grid] - 1;
		distanceToVertWall = (player.pos.x - A_x) / cosf(to_radians(ray_angle));

		*wallPos = GRID_INTERVAL - (int)A_y % GRID_INTERVAL;
	}
	return (distanceToVertWall);
}

