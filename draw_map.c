#include "game_structures.h"

//Player Color = Green
//Exit Color = Yellow
//Enemy Color = Red
//Border Color = Blue

void clearTerminal(void) {
    // \x1b[2J clears the entire screen
    // \x1b[H moves the cursor back to the top-left (home) position
    printf("\x1b[2J\x1b[H");
    fflush(stdout); 
}

void drawMap(char *map, Map *map_size, GameEntity *player, GameEntity *enemy)
{
    // Create Borders

    int borderOffsetX = (map_size->x * 3);
    int borderOffsetY = (map_size->y);

    char borderWidth[borderOffsetX];
    for (int bW = 0; bW < borderOffsetX; bW++)
    {
        borderWidth[bW] = '-';
    }

    char borderHeight[borderOffsetY];
    for (int bH = 0; bH < borderOffsetY; bH++)
    {
        borderHeight[bH] = '|';
    }

    // Draw full scene with grid and Borders

    clearTerminal();

    for (int i = 0; i < ((borderOffsetX / 2) - (7 + level)); i ++)
    {
        printf(" ");
    }
    printf("Level: \e[0;96m%d\e[0m \n", level);

    printf(" ");
    for (int bW = 0; bW < borderOffsetX; bW++)
    {
        printf("\x1b[0;34m%c\x1b[0m", borderWidth[bW]);
    }
    printf("\n");
    for (int y = 0; y < map_size->y; y++) 
    {
        printf("\x1b[0;34m%c\x1b[0m", borderHeight[y]);
        for (int x = 0; x < map_size->x; x++)
        {
            // (y * width) + x = x, y 
            if (map[(y * map_size->x) + x] == player->icon)
            {
                printf(" \x1b[1;92m%c\x1b[0m ", map[(y * map_size->x) + x]);
            }
            else if (map[(y * map_size->x) + x] == enemy->icon)
            {
                printf(" \x1b[1;91m%c\x1b[0m ", map[(y * map_size->x) + x]);
            }
            else if (map[(y * map_size->x) + x] == '$')
            {
                printf(" \x1b[1;93m%c\x1b[0m ", map[(y * map_size->x) + x]);
            }
            else if (map[(y * map_size->x) + x] == '#')
            {
                printf(" \x1b[0;34m%c\x1b[0m ", map[(y * map_size->x) + x]);
            }
            else 
            {
                printf(" %c ", map[(y * map_size->x) + x]);
            }
        }
        printf("\x1b[0;34m%c\x1b[0m", borderHeight[y]);
        printf("\n");
    }
    printf(" ");
    for (int bW = 0; bW < borderOffsetX; bW++)
    {
        printf("\x1b[0;34m%c\x1b[0m", borderWidth[bW]);
    }
    printf("\n");
    
}