#include "game_structures.h"

void updateMap(char *map, Map *map_size, GameEntity *player, GameEntity *enemy)
{
    for (int y = 0; y < map_size->y; y++) // y
    {
        for (int x = 0; x < map_size->x; x++) // x
        {
            if (x == player->position[0] && y == player->position[1])
            {
                map[((y * map_size->x) + x)] = player->icon; // (y * width) + x = x, y 
            }
            else if (x == exit_x && y == exit_y)
            {
                map[((y * map_size->x) + x)] = '$';
            }
            else if (x == enemy->position[0] && y == enemy->position[1])
            {
                map[((y * map_size->x) + x)] = enemy->icon;
            }
            else if (map[((y * map_size->x) + x)] == '#')
            {
                map[((y * map_size->x) + x)] = '#';
            }
            else 
            {
                map[((y * map_size->x) + x)] = ' '; // temporary for testing draw method
            }
        }
    }
}