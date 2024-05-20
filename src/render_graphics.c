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
	int i;
	Maze_player player = {
		{192, 1792},
		
		45
	};

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
 * @player_pos: The position of the player's viewpoint
 * Return: the distance between the ray and the wall
 */
int find_wall(int column, float view_angle, map_location player_pos)
{
	float angle_btwn_rays = (float) FIELD_OF_VIEW / WINDOW_WIDTH;
	float ray_angle;
	float beta; /* distort. cor. factor - angle between ray and viewing angle*/
	float dist_vertical_int = 0; /* distance to vertical wall intersect */
	float dist_horizontal_int = 0; /* distance to horizontal wall intersect */
	float distance; /* Distance from player viewpoint to wall*/
	int dir_y, dir_x; /* Direction of ray in y and x axes*/
	float A_x, A_y; /* Grid intersection coordination */
	int A_x_grid = 0, A_y_grid = 0; /* Grid coordinates */
	bool wall_found;

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
	if (ray_angle > 90 && ray_angle < 270)
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

	while (A_x_grid >= 0 && A_y_grid >= 0)
	{
		/* Find grid coordinates */
		A_y_grid = A_y / GRID_WIDTH;
		A_x_grid = A_x / GRID_WIDTH;
		/* printf("Angle: %f - Hor: Grid [%d, %d] Pos[%f, %f]\n", ray_angle, A_x_grid, A_y_grid, A_x, A_y); */
		wall_found = check_for_wall(A_x_grid, A_y_grid);
		if (wall_found)
			break;
		A_y_grid = (A_y - dir_y) / GRID_WIDTH;
		wall_found = check_for_wall(A_x_grid, A_y_grid);
		if (wall_found)
			break;

		A_x = A_x + (GRID_WIDTH / tanf(to_radians(ray_angle))) * dir_x;
		A_y = A_y + GRID_WIDTH * dir_y;
	}

	if (wall_found)
		dist_vertical_int =  fabs(player_pos.x - A_x) / cosf(to_radians(ray_angle));

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

	A_y_grid = 0;
	A_x_grid = 0;
	while (A_x_grid >= 0 && A_y_grid >= 0)
	{
		A_x_grid = A_x / GRID_WIDTH;
		A_y_grid = A_y / GRID_WIDTH;
		/* printf("Angle: %f - Ver: Grid [%d, %d] Pos[%f, %f]\n", ray_angle, A_x_grid, A_y_grid, A_x, A_y); */
		wall_found = check_for_wall(A_x_grid, A_y_grid);
		if (wall_found)
			break;
		A_x_grid = (A_x - dir_x) / GRID_WIDTH;
		wall_found = check_for_wall(A_x_grid, A_y_grid);
		if (wall_found)
			break;

		A_x = A_x + (GRID_WIDTH * dir_x);
		A_y = A_y + (GRID_WIDTH * tanf(to_radians(ray_angle))) * dir_y;
	}

	if (wall_found)
		dist_horizontal_int =  fabs(player_pos.x - A_x) / cosf(to_radians(ray_angle));

	if (dist_horizontal_int && dist_horizontal_int < dist_vertical_int)
		distance = dist_horizontal_int;
	else
		distance = dist_vertical_int;

	/* Distortion correction */
	beta = fabs(ray_angle - view_angle);
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
	int i;
	Maze_wall_block wall_array[50] = {
		{0, 0},
		{6,17}, {9,17}, {12,17},
		{6,18},	{9,18},	{12,18},
		{6,19},	{9,19},	{12,19},
		{6,20},	{9,20},	{12,20},
		{3,21}, {4,21}, {5,21}, {6,21},	{9,21},	{12,21},
		{9,22},
		{9,23},
		{9,24}, {10,24}, {11,24}, {12,24},
		{12,25},
		{12,26}
	};

	/* Search wall_array for match*/
	for (i = 0; i < 50; i++)
	{
		if (wall_array[i].x == x && wall_array[i].y == y)
		{
			printf("Wall: {%d, %d}\n", x, y);
			return (true);
		}
	}

	return (false);
}

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
