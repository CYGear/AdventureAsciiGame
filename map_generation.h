// map_generation.h
#ifndef MAP_GENERATION_H
#define MAP_GENERATION_H

#include "game_structures.h" // Gives access to Map and GameEntity

void generateMap(char *map, Map *map_size, GameEntity *player, GameEntity *enemy, bool (*isPlayerReachable)(char*, Map*, GameEntity*, GameEntity*), bool (*isExitReachable)(char*, Map*, GameEntity*, GameEntity*));                                     

#endif