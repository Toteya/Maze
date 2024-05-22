#include "../inc/maze.h"

void getWallDistance(RenderColumn *, MazePlayer, int map[][MAP_WIDTH]);
bool check_for_wall(int x, int y, int map[][MAP_WIDTH]);
void draw_wall_slice(RenderColumn, int, SDL_Renderer *);
void set_wall_color(int direction, SDL_Colour *wall_color);

/**
 * render_graphics - Renders the maze graphics onto the screen
 * @gInstance: SDL game instance
 * Return: void
 */
void render_graphics(SDL_Instance *gInstance)
{
	int i;
	int pp_distance; /* Distance between player and project plane */
	RenderColumn column;
	SDL_Renderer *gRenderer = gInstance->renderer;
	MazePlayer player = gInstance->player;

	pp_distance = (WINDOW_WIDTH / 2) / (tanf(to_radians(FIELD_OF_VIEW / 2)));

	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);

	for (i = 0; i <= WINDOW_WIDTH; i++)
	{
		column.index = i;
		getWallDistance(&column, player, gInstance->map);
		/*printf("col: %d, wall distance: %d\n", i, wall_distance);*/

		draw_wall_slice(column, pp_distance, gRenderer);
	}

	SDL_RenderPresent(gRenderer);
}

/**
 * getWallDistance - Calculates the distance of a ray from the player
 * viewpoint to a wall
 * @column: The ray (or column) whose distance to the wall is to be found
 * @p: The game player
 * @map: The wall map array
 * Return: the distance between the ray and the wall
 */
void getWallDistance(RenderColumn *column, MazePlayer p, int map[][MAP_WIDTH])
{
	map_location player_pos = p.pos;
	float view_angle = p.view_angle;
	float angle_b_rays; /* Angle between subsequent rays*/
	float ray_angle;
	float beta; /* distort. cor. factor - angle between ray and viewing angle*/
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
		A_y = (player_pos.y / GRID_INTERVAL) * GRID_INTERVAL - 1;
	else if (dir_y == Y_DIRECTION_DOWN)
		A_y = (player_pos.y / GRID_INTERVAL) * GRID_INTERVAL + GRID_INTERVAL;
	else
	{
		/* ray is purely horizontal -> keep A_y equal player_pos(y) */
		A_y = player_pos.y;
	}
	/* x-cordinate */
	A_x = player_pos.x + (player_pos.y - A_y) / tanf(to_radians(ray_angle));

	wall_found = false;
	while (A_x_grid >= 0 && A_y_grid >= 0)
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

		A_x = A_x + (GRID_INTERVAL / tanf(to_radians(ray_angle))) * dir_x;
		A_y = A_y + GRID_INTERVAL * dir_y;
	}

	if (wall_found)
		dist_hor_int =  fabs(player_pos.x - A_x) / cosf(to_radians(ray_angle));

	/* find a vertical intersection with a wall */
	/* x-coordinate */
	if (dir_x == X_DIRECTION_LEFT)
		A_x = (player_pos.x / GRID_INTERVAL) * GRID_INTERVAL - 1;
	else if (dir_x == X_DIRECTION_RIGHT)
		A_x = (player_pos.x / GRID_INTERVAL) * GRID_INTERVAL + GRID_INTERVAL;
	else
		A_x = player_pos.x; /* The ray is purely vertical -> don't change */
	/* y-coordinate*/
	A_y = player_pos.y + (player_pos.x - A_x) * tanf(to_radians(ray_angle));

	A_y_grid = 0;
	A_x_grid = 0;
	wall_found = false;
	while (A_x_grid >= 0 && A_y_grid >= 0)
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
		A_y = A_y + (GRID_INTERVAL * tanf(to_radians(ray_angle))) * dir_y;
	}

	if (wall_found)
		dist_ver_int =  fabs(player_pos.x - A_x) / cosf(to_radians(ray_angle));

	/* Not to address potential bug - CASE: distance is correctly ZERO*/
	if (dist_hor_int > 0)
	{
		distance = dist_hor_int;
		if (dir_y == Y_DIRECTION_UP)
			column->direction = SOUTH;
		else
			column->direction = NORTH;
	}
	else
	{
		distance = dist_ver_int;
		if (dir_x == X_DIRECTION_LEFT)
			column->direction = EAST;
		else
			column->direction = WEST;
	}
	if (dist_ver_int > 0 && dist_ver_int < dist_hor_int)
	{
		distance = dist_ver_int;
		if (dir_x == X_DIRECTION_LEFT)
			column->direction = EAST;
		else
			column->direction = WEST;
	}

	/* Distortion correction */
	beta = fabs(ray_angle - view_angle);
	if (beta > 180)
		beta = 360 - beta;

	column->distance = distance * cosf(to_radians(beta));
	/**
	* printf("col: %d, d_hor: %f, d_ver: %f, d: %f, d_cor: %d, dir: %d\n",
	* column->index, dist_hor_int, dist_ver_int, distance,
	* column->direction, column->direction);
	*/
}

/**
 * check_for_wall - Checks if there is a wall at the given grid position
 * @x: The x coordinate of the grid position to be checked.
 * @y: The y coordinate of the grid position to be checked
 * @map: The wall map array
 * Return: (bool) TRUE if there is a wall. Otherwise return FALSE.
 */
bool check_for_wall(int x, int y, int map[][MAP_WIDTH])
{
	if (!((x >= 0 && x < 30) && (y >= 0 && y < 30)))
		return (false);

	if (map[y][x])
		return (true);

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

/**
 * draw_wall_slice - Draws a wall slice onto the projection plane
 * @pp_dist: The distance from the player to the projection plane
 * @c: The column to be rendered
 * @gRenderer: The SDL renderer
 * Return: Nothing
 */
void draw_wall_slice(RenderColumn c, int pp_dist, SDL_Renderer *gRenderer)
{
	int wall_height = GRID_INTERVAL; /* Actual wall height*/
	int proj_height; /* Wall projection height */
	int start_point, end_point;
	SDL_Color w_color;

	proj_height = wall_height * ((float) pp_dist / c.distance);
	/**
	* printf("col: %d, Dw: %d, Dp: %d, Hw: %d, Hp: %d\n",
	* column, wall_distance, pp_distance, wall_height, proj_height);
	*/

	start_point = (WINDOW_HEIGHT / 2) - (proj_height / 2);
	end_point = start_point + proj_height;

	set_wall_color(c.direction, &w_color);
	SDL_SetRenderDrawColor(gRenderer, w_color.r, w_color.g, w_color.g, 0xFF);
	SDL_RenderDrawLine(gRenderer, c.index, start_point, c.index, end_point);
}

/**
 * set_wall_color - Sets the wall shade based on the direction the wall
 * is facing
 * @direction: The direction that the wall is facing
 * @wall_color: The wall color to be updated
 * Return: Nothing
 */
void set_wall_color(int direction, SDL_Colour *wall_color)
{
	switch (direction)
	{
		case NORTH:
			wall_color->r = 0xAF;
			wall_color->g = 0xA5;
			wall_color->b = 0x96;
			wall_color->a = 0xFF;
			break;
		case SOUTH:
			wall_color->r = 0xAF;
			wall_color->g = 0xA5;
			wall_color->b = 0x96;
			wall_color->a = 0xFF;
			break;
		case EAST:
			wall_color->r = 0x5A;
			wall_color->g = 0x55;
			wall_color->b = 0x4D;
			wall_color->a = 0xFF;
			break;
		case WEST:
			wall_color->r = 0x5A;
			wall_color->g = 0x55;
			wall_color->b = 0x4D;
			wall_color->a = 0xFF;
			break;
		default:
			break;
	}
}
