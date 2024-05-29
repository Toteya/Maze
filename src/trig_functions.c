#include "../inc/maze.h"

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
