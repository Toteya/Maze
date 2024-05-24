#include "../inc/maze.h"

/**
 * init_map - Initialises the maze grid map.
 * @map: A 2D-array (representing a map) to be initialised
 * Return: (bool) TRUE on success. Otherwise return FALSE.
 * Description: This function initialises the maze grid as an array of
 * wall_blocks. Each wall block has grid coordinates of its position on the map
 */
bool init_map(int map[][MAP_WIDTH], char *filename)
{
	FILE *fp;
	char buff[MAP_WIDTH * 3]; /* Sufficiently large buffer for one line of the file */
	char *token = NULL;
	int i, j;

	fp = fopen(filename, "r");
	if (fp == NULL)
	{
		fprintf(stderr, "Could not open file %s\n", filename);
		return false;
	}

	i = 0;
	while (fgets(buff, sizeof(buff), fp) != NULL)
	{
		j = 0;
		token = strtok(buff, ",");
		while (token != NULL)
		{
			map[i][j++] = atoi(token);
			token = strtok(NULL, ",");
		}
		i++;
	}
	fclose(fp);

	return (true);
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
	{
		/* printf("wall: [%d, %d]\n", x, y); */
		return (true);
	}

	return (false);
}
