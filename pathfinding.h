// pathfinding.h
#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "game_structures.h" // Gives access to Map and GameEntity

bool isExitReachable(char *map, Map *map_size, GameEntity *player, GameEntity *enemy);// used to check if player can get to exit
bool isPlayerReachable(char *map, Map *map_size, GameEntity *player, GameEntity *enemy); // used to check if enemy can get to player
void findPlayerPath(char *map, Map *map_size, GameEntity *player, GameEntity *enemy, BotDirection *botDirCache, int *botCacheSize); // Used to find path from enemy to player and set bot direction cache to that value

// Also, only run isPlayerReachable on map generation.
// Only run findPlayerPath if player moves, else
// just leave it in the cache/variable for the data

#endif