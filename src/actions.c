#include "../inc/maze.h"

/**
 * do_action - Performs an action according to the given action code
 * @action: The code specifying the action to performed
 * @player: The player
 * Return: Nothing
 */
void do_action(int action, Maze_player *player)
{
    switch (action)
    {
    case ACTION_LEFT:
        do_turn(action, player);
        break;
    case ACTION_RIGHT:
        do_turn(action, player);
        break;
    default:
        break;
    }   
}


/**
 * do_turn - Rotates the player's viewpoint according to the given direction
 * of rotation
 * @action: the action code specifying the direction of rotation
 * @player: the player
 * Return: Nothing
 */
void do_turn(int action, Maze_player *player)
{
	if (action == ACTION_LEFT)
        player->view_angle++;
    else
        player->view_angle--;
}
