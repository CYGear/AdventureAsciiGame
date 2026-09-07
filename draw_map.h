// draw_map.h
#ifndef DRAW_MAP_H
#define DRAW_MAP_H

#include "game_structures.h" // Gives access to Map and GameEntity

void clearTerminal();

void drawMap(char *map, Map *map_size, GameEntity *player, GameEntity *enemy); // Only need map and player, player to change color of player icon when drawn

#endif