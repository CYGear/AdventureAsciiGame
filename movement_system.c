#include "game_structures.h"

void enable_raw_mode(struct termios *orig) {
    struct termios raw;
    tcgetattr(STDIN_FILENO, orig);
    raw = *orig;
    // Disable canonical mode (line buffering) and echo
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void disable_raw_mode(struct termios *orig) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, orig);
}

MoveStatus checkMove(char *map, Map *map_size, GameEntity *player, GameEntity *enemy, char direction, char who)
{
    int pos_x = 0, pos_y = 0;
    int deathChar = ' '; // The character in which it will return DEAD if is the next character

    if (who == 'P') // If player    
    {
        pos_x = player->position[0];
        pos_y = player->position[1];

        deathChar = '!'; // if player next move is on enemy, player dies
    }
    else
    {
        pos_x = enemy->position[0];
        pos_y = enemy->position[1];

        deathChar = player->icon; // if enemy next move is on player, player dies
    }
    switch (direction)
    {
        case 'W': // | (y * width) + x = x, y | Just the formula thing
            // First check for out of bounds
            if ((pos_y - 1) >= 0)
            {
                if (map[((pos_y - 1) * map_size->x) + pos_x] == '#') // player pos - 1 to go up in y
                {
                    return BLOCKED;
                }
                else if (map[((pos_y - 1) * map_size->x) + pos_x] == '$')
                {
                    return EXITED;
                }
                else if (map[((pos_y - 1) * map_size->x) + pos_x] == deathChar)
                {
                    return DEAD;
                }
                else
                {
                    return VALID;
                }
            }
            else
            {
                return BLOCKED;
            }
            break;
        case 'S': 
            if ((pos_y + 1) < map_size->y)
            {
                if (map[((pos_y + 1) * map_size->x) + pos_x] == '#') // player pos + 1 to go down in y
                {
                    return BLOCKED;
                }
                else if (map[((pos_y + 1) * map_size->x) + pos_x] == '$')
                {
                    return EXITED;
                }
                else if (map[((pos_y + 1) * map_size->x) + pos_x] == deathChar)
                {
                    return DEAD;
                }
                else if (map[((pos_y + 1) * map_size->x) + pos_x] == ' ')
                {
                    return VALID;
                }
            }
            else
            {
                return BLOCKED;
            }
            break;
        case 'A':
            if ((pos_x - 1) >= 0)
            {
                if (map[(pos_y * map_size->x) + (pos_x - 1)] == '#') // player pos - 1 to go left in x
                {
                    return BLOCKED;
                }
                else if (map[(pos_y * map_size->x) + (pos_x - 1)] == '$')
                {
                    return EXITED;
                }
                else if (map[(pos_y * map_size->x) + (pos_x - 1)] == deathChar)
                {
                    return DEAD;
                }
                else if (map[(pos_y * map_size->x) + (pos_x - 1)] == ' ')
                {
                    return VALID;
                }
            }
            else
            {
                return BLOCKED;
            }
            break;
        case 'D':
            if ((pos_x + 1) < map_size->x)
            {
                if (map[(pos_y * map_size->x) + (pos_x + 1)] == '#') // player pos + 1 to go right in x
                {
                    return BLOCKED;
                }
                else if (map[(pos_y * map_size->x) + (pos_x + 1)] == '$')
                {
                    return EXITED;
                }
                else if (map[(pos_y * map_size->x) + (pos_x + 1)] == deathChar)
                {
                    return DEAD;
                }
                else if (map[(pos_y * map_size->x) + (pos_x + 1)] == ' ')
                {
                    return VALID;
                }
            }
            else
            {
                return BLOCKED;
            }
            break;

        default: return INVALID_INPUT; break;
    }
    return INVALID_INPUT;
}

MoveStatus inputLoop(char *map, Map *map_size, GameEntity *player, GameEntity *enemy, void (*updateMap)(char*, Map*, GameEntity*, GameEntity*), void (*drawMap)(char*, Map*, GameEntity*, GameEntity*), void (*findPlayerPath)(char*, Map*, GameEntity*, GameEntity*, BotDirection*, int*), BotDirection *botDirCache, int *botCacheSize)
{
    bool isRunning = true;

    struct termios orig_settings;
    enable_raw_mode(&orig_settings);

    do
    {
        updateMap(map, map_size, player, enemy);
        drawMap(map, map_size, player, enemy);

        char move = getchar(); // WASD

        MoveStatus move_status = VALID;

        switch (toupper(move))
        {
            case 'W': 
                move_status = checkMove(map, map_size, player, enemy, 'W', 'P'); // Check player move
        
                if (move_status == VALID) // If move is valid, we need change player position to new spot and 
                {                               // call the drawMap function
                    player->position[1] -= 1; // | (y * width) + x = x, y | Just the formula thing

                    playerBlockedCounter = 0; // Reset because player just moved

                    // WE HAVE TO NOW USE findPlayerPath and move enemy
                    findPlayerPath(map, map_size, player, enemy, botDirCache, botCacheSize); // sets botDirCache to have player path
                        
                    if (botDirCache[playerBlockedCounter] == UP) // pBC will be 0 but this is just for readability
                    {
                        enemy->position[1] -= 1;
                    }
                    else if (botDirCache[playerBlockedCounter] == DOWN)
                    {
                        enemy->position[1] += 1;
                    }
                    else if (botDirCache[playerBlockedCounter] == LEFT)
                    {
                        enemy->position[0] -= 1;
                    }
                    else if (botDirCache[playerBlockedCounter] == RIGHT)
                    {
                        enemy->position[0] += 1;
                    }

                    updateMap(map, map_size, player, enemy);

                }
                else if (move_status == BLOCKED)
                {
                    // we need a global playerBlockedCounter variable to see how many times the player has not moved/blocked and move the enemy using the botDirCache
                        // at the index of how many times the player has not moved.
                    playerBlockedCounter += 1;

                    if (botDirCache[playerBlockedCounter] == UP) 
                    {
                        enemy->position[1] -= 1;
                    }
                    else if (botDirCache[playerBlockedCounter] == DOWN)
                    {
                        enemy->position[1] += 1;
                    }
                    else if (botDirCache[playerBlockedCounter] == LEFT)
                    {
                        enemy->position[0] -= 1;
                    }
                    else if (botDirCache[playerBlockedCounter] == RIGHT)
                    {
                        enemy->position[0] += 1;
                    }

                    updateMap(map, map_size, player, enemy);
                    
                }
                else if (move_status == EXITED)
                {
                    player->position[1] -= 1; 
                    updateMap(map, map_size, player, enemy);
                    return EXITED;
                }
                else if (move_status == DEAD)
                {
                    return DEAD;
                }
                break;
            case 'S':
                move_status = checkMove(map, map_size, player, enemy, 'S', 'P'); 
        
                if (move_status == VALID)
                {
                    player->position[1] += 1; 
                    playerBlockedCounter = 0; // Reset because player just moved

                    // WE HAVE TO NOW USE findPlayerPath and move enemy
                    findPlayerPath(map, map_size, player, enemy, botDirCache, botCacheSize); // sets botDirCache to have player path
                        
                    if (botDirCache[playerBlockedCounter] == UP) // pBC will be 0 but this is just for readability
                    {
                        enemy->position[1] -= 1;
                    }
                    else if (botDirCache[playerBlockedCounter] == DOWN)
                    {
                        enemy->position[1] += 1;
                    }
                    else if (botDirCache[playerBlockedCounter] == LEFT)
                    {
                        enemy->position[0] -= 1;
                    }
                    else if (botDirCache[playerBlockedCounter] == RIGHT)
                    {
                        enemy->position[0] += 1;
                    }

                    updateMap(map, map_size, player, enemy);
                }
                else if (move_status == BLOCKED)
                {
                    // we need a global playerBlockedCounter variable to see how many times the player has not moved/blocked and move the enemy using the botDirCache
                        // at the index of how many times the player has not moved.
                    playerBlockedCounter += 1;

                    if (botDirCache[playerBlockedCounter] == UP) 
                    {
                        enemy->position[1] -= 1;
                    }
                    else if (botDirCache[playerBlockedCounter] == DOWN)
                    {
                        enemy->position[1] += 1;
                    }
                    else if (botDirCache[playerBlockedCounter] == LEFT)
                    {
                        enemy->position[0] -= 1;
                    }
                    else if (botDirCache[playerBlockedCounter] == RIGHT)
                    {
                        enemy->position[0] += 1;
                    }

                    updateMap(map, map_size, player, enemy);
                    
                }
                else if (move_status == EXITED)
                {
                    player->position[1] += 1; 
                    updateMap(map, map_size, player, enemy);
                    return EXITED;
                }
                else if (move_status == DEAD)
                {
                    return DEAD;
                }
                break;
            case 'A':
                move_status = checkMove(map, map_size, player, enemy, 'A', 'P'); 
        
                if (move_status == VALID)
                {
                    player->position[0] -= 1; 
                    playerBlockedCounter = 0; // Reset because player just moved

                    // WE HAVE TO NOW USE findPlayerPath and move enemy
                    findPlayerPath(map, map_size, player, enemy, botDirCache, botCacheSize); // sets botDirCache to have player path
                        
                    if (botDirCache[playerBlockedCounter] == UP) // pBC will be 0 but this is just for readability
                    {
                        enemy->position[1] -= 1;
                    }
                    else if (botDirCache[playerBlockedCounter] == DOWN)
                    {
                        enemy->position[1] += 1;
                    }
                    else if (botDirCache[playerBlockedCounter] == LEFT)
                    {
                        enemy->position[0] -= 1;
                    }
                    else if (botDirCache[playerBlockedCounter] == RIGHT)
                    {
                        enemy->position[0] += 1;
                    }

                    updateMap(map, map_size, player, enemy);
                }
                else if (move_status == BLOCKED)
                {
                    // we need a global playerBlockedCounter variable to see how many times the player has not moved/blocked and move the enemy using the botDirCache
                        // at the index of how many times the player has not moved.
                    playerBlockedCounter += 1;

                    if (botDirCache[playerBlockedCounter] == UP) 
                    {
                        enemy->position[1] -= 1;
                    }
                    else if (botDirCache[playerBlockedCounter] == DOWN)
                    {
                        enemy->position[1] += 1;
                    }
                    else if (botDirCache[playerBlockedCounter] == LEFT)
                    {
                        enemy->position[0] -= 1;
                    }
                    else if (botDirCache[playerBlockedCounter] == RIGHT)
                    {
                        enemy->position[0] += 1;
                    }

                    updateMap(map, map_size, player, enemy);
                    
                }
                else if (move_status == EXITED)
                {
                    player->position[0] -= 1; 
                    updateMap(map, map_size, player, enemy);
                    return EXITED;
                }
                else if (move_status == DEAD)
                {
                    return DEAD;
                }
                break;
            case 'D':
                move_status = checkMove(map, map_size, player, enemy, 'D', 'P'); 
        
                if (move_status == VALID)
                {
                    player->position[0] += 1; 
                    playerBlockedCounter = 0; // Reset because player just moved

                    // WE HAVE TO NOW USE findPlayerPath and move enemy
                    findPlayerPath(map, map_size, player, enemy, botDirCache, botCacheSize); // sets botDirCache to have player path
                        
                    if (botDirCache[playerBlockedCounter] == UP) // pBC will be 0 but this is just for readability
                    {
                        enemy->position[1] -= 1;
                    }
                    else if (botDirCache[playerBlockedCounter] == DOWN)
                    {
                        enemy->position[1] += 1;
                    }
                    else if (botDirCache[playerBlockedCounter] == LEFT)
                    {
                        enemy->position[0] -= 1;
                    }
                    else if (botDirCache[playerBlockedCounter] == RIGHT)
                    {
                        enemy->position[0] += 1;
                    }

                    updateMap(map, map_size, player, enemy);
                }
                else if (move_status == BLOCKED)
                {
                    // we need a global playerBlockedCounter variable to see how many times the player has not moved/blocked and move the enemy using the botDirCache
                        // at the index of how many times the player has not moved.
                    playerBlockedCounter += 1;

                    if (botDirCache[playerBlockedCounter] == UP) 
                    {
                        enemy->position[1] -= 1;
                    }
                    else if (botDirCache[playerBlockedCounter] == DOWN)
                    {
                        enemy->position[1] += 1;
                    }
                    else if (botDirCache[playerBlockedCounter] == LEFT)
                    {
                        enemy->position[0] -= 1;
                    }
                    else if (botDirCache[playerBlockedCounter] == RIGHT)
                    {
                        enemy->position[0] += 1;
                    }

                    updateMap(map, map_size, player, enemy);
                    
                }
                else if (move_status == EXITED)
                {
                    player->position[0] += 1; 
                    updateMap(map, map_size, player, enemy);
                    return EXITED;
                }
                else if (move_status == DEAD)
                {
                    return DEAD;
                }
                break;
    }
    
    }while (isRunning);

    disable_raw_mode(&orig_settings);

    return DEAD;
}
