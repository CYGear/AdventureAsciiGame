#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>

#ifndef GAME_STRUCTURES_H
#define GAME_STRUCTURES_H

extern int exit_x;
extern int exit_y;

extern int level;

extern int playerBlockedCounter; // used to find path from enemy to player if this is equal to 0

typedef enum
{
    UP,
    DOWN,
    LEFT,
    RIGHT,

} BotDirection;

typedef enum 
{
    VALID, // Yes move, nothing happens
    BLOCKED, //Blocked by a wall
    EXITED, // Reached exit, if reach exit we will run resizeMap function to add 2 to its size and rellocate memory
    DEAD, // Touched enemy, enemy touch player
    INVALID_INPUT, // Invalid input

} MoveStatus;

typedef struct
{
    int position[2]; // x, y
    char icon;
    

} GameEntity;

typedef struct 
{
    size_t x, y;
    
} Map;

#endif