#include "../inc/maze.h"

int find_wall_distance(int, float, map_location);
float to_radians(float angle_deg);
bool check_for_wall(int x, int y);
int draw_wall_slice(int column, int wall_distance, int pp_distance, SDL_Renderer *);

/**
 * render_graphics - Renders the maze graphics onto the screen
 *
 * Return: void
 */
void render_graphics(SDL_Renderer *gRenderer)
{
	int i;
	int wall_distance; /* Distance between player and wall */
	int pp_distance; /* Distance between player and project plane */
	Maze_player player = {
		{192, 1792},
		
		45
	};

	pp_distance = (WINDOW_WIDTH / 2) / (tanf(to_radians(FIELD_OF_VIEW / 2)));

	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(gRenderer);

	for (i = 0; i <= WINDOW_WIDTH; i++)
	{
		wall_distance = find_wall_distance(i, player.view_angle, player.pos);
		/*printf("col: %d, wall distance: %d\n", i, wall_distance);*/
		
		draw_wall_slice(i, wall_distance, pp_distance, gRenderer);
			
	}

	SDL_RenderPresent(gRenderer);
}

/**
 * find_wall - Calculates the distance of a ray from the player viewpoint
 * to a wall
 * @column: The ray (or column) whose distance to the wall is to be found
 * @view_angle: The player's angle of view.
 * @player_pos: The position of the player's viewpoint
 * Return: the distance between the ray and the wall
 */
int find_wall_distance(int column, float view_angle, map_location player_pos)
{
	float angle_btwn_rays = (float) FIELD_OF_VIEW / WINDOW_WIDTH;
	float ray_angle;
	float beta; /* distort. cor. factor - angle between ray and viewing angle*/
	float dist_ver_int = 0; /* distance to vertical wall intersect */
	float dist_hor_int = 0; /* distance to horizontal wall intersect */
	float distance; /* Distance from player viewpoint to wall*/
	float cor_distance; /* Distance corrected for distortion viewpoint to wall*/
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

	wall_found = false;
	while (A_x_grid >= 0 && A_y_grid >= 0)
	{
		/* Find grid coordinates */
		A_y_grid = A_y / GRID_WIDTH;
		A_x_grid = A_x / GRID_WIDTH;
		/*printf("col: %d, Angle: %f - Hor: Grid [%d, %d] Pos[%f, %f]\n", column, ray_angle, A_x_grid, A_y_grid, A_x, A_y);*/
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
		dist_hor_int =  fabs(player_pos.x - A_x) / cosf(to_radians(ray_angle));

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
	wall_found = false;
	while (A_x_grid >= 0 && A_y_grid >= 0)
	{
		A_x_grid = A_x / GRID_WIDTH;
		A_y_grid = A_y / GRID_WIDTH;
		/*printf("col: %d, Angle: %f - Ver: Grid [%d, %d] Pos[%f, %f]\n", column, ray_angle, A_x_grid, A_y_grid, A_x, A_y);*/
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
		dist_ver_int =  fabs(player_pos.x - A_x) / cosf(to_radians(ray_angle));

	/* Not to address potential bug - CASE: distance is correctly ZERO*/
	if (dist_hor_int > 0)
		distance = dist_hor_int;
	else
		distance = dist_ver_int;
	if (dist_ver_int > 0 && dist_ver_int < dist_hor_int)
		distance = dist_ver_int;
	

	/* Distortion correction */
	beta = fabs(ray_angle - view_angle);
	if (beta > 180)
		beta = 360 - beta;

	cor_distance = distance * cosf(to_radians(beta));
	/*printf("d_hor: %f, d_ver: %f, d: %f, d_cor: %f\n", dist_hor_int, dist_ver_int, distance, cor_distance);*/

	return (cor_distance);
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
			/* printf("Wall: {%d, %d}\n", x, y); */
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

/**
 * draw_wall_slice - Draws a wall slice onto the projection plane
 * @wall_distance: The distance from the player's viewpoint to the wall
 * @pp_distance: The distance from the player to the projection plane
 * Return: 
 */
int draw_wall_slice(int column, int wall_distance, int pp_distance, SDL_Renderer *gRenderer)
{
	int wall_height = GRID_WIDTH; /* Actual wall height*/
	int proj_height; /* Wall projection height */
	int start_point, end_point;

	proj_height = wall_height * ((float) pp_distance / wall_distance);
	/*printf("col: %d, Dw: %d, Dp: %d, Hw: %d, Hp: %d\n", column, wall_distance, pp_distance, wall_height, proj_height);*/
	
	start_point = (WINDOW_HEIGHT / 2) - (proj_height / 2);
	end_point = start_point + proj_height;

	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderDrawLine(gRenderer, column, start_point, column, end_point);

	return (wall_distance);
}
