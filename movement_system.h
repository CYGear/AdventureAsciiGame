// movement_system.h
#ifndef MOVEMENT_SYSTEM_H
#define MOVEMENT_SYSTEM_H

#include "game_structures.h" // Gives access to Map and GameEntity

// We need to get input keys: W A S D store as enum for readability, Use getchar function for keyboard input
// We need to check what is on the next player location
// If next object is a wall, prevent that move and make enemy move
// If next object is the exit, use the nextLevel() function that will:
// Reallocate memory to resize array grid map by 2, so instead of 10 by 10 it is 12 by 12
// If next object is enemy then program exits, and same if enemy's next move is on you

MoveStatus checkMove(char *map, Map *map_size, GameEntity *player, GameEntity *enemy, char direction, char who);

MoveStatus inputLoop(char *map, Map *map_size, GameEntity *player, GameEntity *enemy, void (*updateMap)(char*, Map*, GameEntity*, GameEntity*), void (*drawMap)(char*, Map*, GameEntity*, GameEntity*), void (*findPlayerPath)(char*, Map*, GameEntity*, GameEntity*, BotDirection*, int*));
                                                                                                                                                                                            //void findPlayerPath(char *map, Map *map_size, GameEntity *player, GameEntity *enemy, BotDirection *botDirCache, int *botCacheSize);

#endif