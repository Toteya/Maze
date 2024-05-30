#include "../inc/maze.h"

int getFloorDistance(int fp_row, int pp_distance, float beta);
void renderFloorPixel(SDL_Instance *gInstance, int x, int y, int f_x, int f_y);

/**
 * drawFloorSlice - Draws the floor for the given column of the window
 * @gInstance: The maze game SDL instance
 * @column: The column to be rendered
 * @player: The player
 * @pp_distance: The distance between the player and the projection plane
 */
void drawFloorSlice(SDL_Instance *gInstance, RendColumn column,
	MazePlayer player, int pp_distance)
{
	int fp_row; /* The projected floor point row */
	int distance; /* Distance to floor point */
	float beta; /* Angle between ray angle and viewing angle*/
	int F_x, F_y; /* x- and y- coordinates of the floor point */

	beta = fabs(column.ray_angle - player.view_angle);
	if (beta > 180)
		beta = 360 - beta;

	fp_row = column.wb_row;

	while (fp_row <= WINDOW_HEIGHT)
	{
		distance = getFloorDistance(fp_row, pp_distance, beta);
		F_x = roundf(player.pos.x + distance * cosf(to_radians(column.ray_angle)));
		F_y = roundf(player.pos.y - distance * sinf(to_radians(column.ray_angle)));
		/**
		* printf("dc: %d, d: %d, alpha: %d, P: [%d, %d], F: [%d, %d]\n",
		* column.distance, distance, column.ray_angle,
		* player.pos.x, player.pos.y, F_x, F_y);
		*/
		renderFloorPixel(gInstance, column.index, fp_row, F_x, F_y);
		fp_row++;
	}
}

/**
 * getFloorDistance - Calculates the distance on the map from the player to
 * the floor point
 * @fp_row: The row of the projected floor point
 * @pp_distance: The distance between the player and the projection plane
 * @beta: The angle between the ray angle and the viewing angle
 * Return: the distance from the player to t
 */
int getFloorDistance(int fp_row, int pp_distance, float beta)
{
	int r; /* Difference between projected floor point and player height */
	int s_distance; /* straight distance from player to the floor point */
	int distance; /* True distance from player to the floor point */
	int p_height; /* Player height */

	p_height = WALL_HEIGHT / 2;
	r = fp_row - (WINDOW_HEIGHT / 2);

	s_distance = roundf(pp_distance * ((float) p_height / r));
	distance = roundf(s_distance / (cosf(to_radians(beta))));

	/**
	* printf("d: %d, d_s: %d, beta: %f, dp: %d, fp: %d, p_height: %d\n",
	* distance, s_distance, beta, pp_distance, fp_row, p_height);
	*/
	return (distance);
}

/**
 * renderFloorPixel - Renders a pixel of the floor texture based on the
 * given floor position
 * @gInstance: The maze game SDL instance
 * @x: The x position on the screen to render
 * @y: The y position on the screen to render
 * @f_x: The x- coordinate of the floor point
 * @f_y: The y- coordinate of the floor point
 */
void renderFloorPixel(SDL_Instance *gInstance, int x, int y, int f_x, int f_y)
{
	SDL_Renderer *gRenderer = gInstance->renderer;
	M_Texture *fTexture = &(gInstance->floor_texture);

	SDL_Rect renderPX; /* Destination pixel on screen */
	SDL_Rect clipPX; /* The source texture pixel */
	int t_x, t_y; /* The texture pixel coordinates */

	t_x = fTexture->width * fabs((float)(f_x % GRID_INTERVAL) / GRID_INTERVAL);
	t_y = fTexture->height * fabs((float)(f_y % GRID_INTERVAL) / GRID_INTERVAL);
	/**
	* printf("screen: [%d, %d], floor: [%d, %d], texture: [%d, %d]\n\n",
	* x, y, f_x, f_y, t_x, t_y);
	*/
	clipPX.x = t_x;
	clipPX.y = t_y;
	clipPX.w = 1;
	clipPX.h = 1;

	renderPX.x = x;
	renderPX.y = y;
	renderPX.w = 1;
	renderPX.h = 1;

	SDL_RenderCopy(gRenderer, fTexture->mTexture, &clipPX, &renderPX);
}

/**
 * paintFloor - Renders the default floor color. This color will be visible
 * if no texture is rendered
 * @gRenderer: The SDL_Renderer
 */
void paintFloor(SDL_Renderer *gRenderer)
{
	SDL_Rect floor = {
		0,
		WINDOW_HEIGHT / 2,
		WINDOW_WIDTH,
		WINDOW_HEIGHT / 2
	};

	SDL_SetRenderDrawColor(gRenderer, 0xEF, 0xB9, 0x6E, 0xFF);
	SDL_RenderFillRect(gRenderer, &floor);
}
