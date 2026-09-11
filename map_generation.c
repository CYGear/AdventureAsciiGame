#include "game_structures.h"

typedef enum
{
    VERTICAL, // 3 # in y
    HORIZONTAL, // 3 # in x

} WallDirection;

typedef struct 
{
    WallDirection wallDir;
    int x; // if horizontal it is x + 2
    int y; // if vertical it is y + 2

} Wall;

void placeGameObjects(char *map, Map *map_size, GameEntity *player, GameEntity *enemy, bool isPlayerPos, bool isEnemyPos, bool isExitPos)
{
    printf("\nSTARTED  PLACING ENEMY, PLAYER, AND EXIT!");
    for (int y = 0; y < map_size->y; y++) // y
    {
        for (int x = 0; x < map_size->x; x++) // x
        {
            isPlayerPos = (x == player->position[0] && y == player->position[1]);
            isEnemyPos = (x == enemy->position[0] && y == enemy->position[1]);
            isExitPos = (x == exit_x && y == exit_y);

            map[((player->position[1] * map_size->x) + player->position[0])] = player->icon;
            map[((enemy->position[1] * map_size->x) + enemy->position[0])] = enemy->icon;
            map[((exit_y * map_size->x) + exit_x)] = '$';

            if (!isExitPos && !isPlayerPos && !isEnemyPos)
            {
                map[((y * map_size->x) + x)] = ' '; // temporary for testing draw method
            }
        }
    }
    printf("\nFINNISHED PLACING ENEMY, PLAYER, AND EXIT!");
}

void clearMap(char *map, Map *map_size)
{
    printf("\nSTARTED CLEARING");
    for (int y = 0; y < map_size->y; y++) // y
    {
        for (int x = 0; x < map_size->x; x++) // x
        {
            map[((y * map_size->x) + x)] = ' '; // reset to blank
        }
    }
    printf("\nFINNISHED CLEARING MAP!");
}

void generateMap(char *map, Map *map_size, GameEntity *player, GameEntity *enemy, bool (*isPlayerReachable)(char*, Map*, GameEntity*, GameEntity*), bool (*isExitReachable)(char*, Map*, GameEntity*, GameEntity*))
{
    player->position[0] = 0;
    player->position[1] = 0;

    enemy->position[0] = (map_size->x - 1);
    enemy->position[1] = (map_size->y - 1); // 1 less than the full map size, so that it stays in bounds

    srand(time(NULL));

    do
    {
        exit_x = rand() % map_size->x; // number between 0 and 1 less than the map_size->x
        exit_y = rand() % map_size->y;

    }while (((exit_x == player->position[0]) && (exit_y == player->position[1])) || ((exit_x == enemy->position[0]) && (exit_y == enemy->position[1])));
    printf("\n%d, %d", exit_x, exit_y);

    // RULES:
    // PLAYER x, y = (0, 0)
    // ENEMY x, y = ((map_size->map_size_x - 1), map_size->map_size_y - 1))
    // EXIT x, y = RANDOM
    // WALL = Will be placed as ### 3 wall characters is equal to a wall.
    //        We need to randomly generate them based on a set of rules.
    //        Make sure it is possible to have player reach exit, and enemy reach player.
    //        Change amount of walls generated based on level/size of map.
    //        EX: size is 10 * 10 we will generate 3 or 4 walls, if bigger than more.
    //        We need to have some sort of pathfinding for enemy and resuse it for wall generation.
    //        CANNOT BE IN THE SAME PLACE AS PLAYER, ENEMY, OR EXIT.

    // WALLS:
    
    int wallsToGenerate = map_size->x; // is 10 * 10 then generate 10 wall coords in entire array
    int wallsGenerated = 0;

    int tempX = 0;
    int tempY = 0;

    bool conditionVert1 = false;
    bool conditionVert2 = false;
    bool conditionVert3 = false;

    bool conditionHori1 = false;
    bool conditionHori2 = false;
    bool conditionHori3 = false;

    bool isPlayerPos = false;
    bool isEnemyPos = false;
    bool isExitPos = false;

    printf("\nSTARTED WALL GENERATION!");
    do
    {
        if (!isPlayerReachable(map, map_size, player, enemy) || !isExitReachable(map, map_size, player, enemy))
        {
            clearMap(map, map_size);
            placeGameObjects(map, map_size, player, enemy, isPlayerPos, isEnemyPos, isExitPos);

            wallsGenerated = 0;
        }

        do
        {
            printf("\nWalls generated: %d out of %d", wallsGenerated, wallsToGenerate);

            tempX = rand() % map_size->x;
            tempY = rand() % map_size->y;

            isPlayerPos = (tempX == player->position[0] && tempY == player->position[1]);
            isEnemyPos = (tempX == enemy->position[0] && tempY == enemy->position[1]);
            isExitPos = (tempX == exit_x && tempY == exit_y);

            conditionVert1 = (map[((tempY * map_size->x) + tempX)] == ' ');
            conditionVert2 = (map[(((tempY + 1) * map_size->x) + tempX)] == ' ');
            conditionVert3 = (map[(((tempY + 2) * map_size->x) + tempX)] == ' ');

            conditionHori1 = (map[((tempY * map_size->x) + tempX)] == ' ');
            conditionHori2 = (map[((tempY * map_size->x) + (tempX + 1))] == ' ');
            conditionHori3 = (map[((tempY * map_size->x) + (tempX + 2))] == ' ');

            WallDirection tempDir = rand() % 2;

            if (tempDir == VERTICAL)
            {
                if ((conditionVert1 && conditionVert2 && conditionVert3) && !(isPlayerPos && isEnemyPos && isExitPos))
                {
                    map[((tempY * map_size->x) + tempX)] = '#';
                    map[(((tempY + 1) * map_size->x) + tempX)] = '#';
                    map[(((tempY + 2) * map_size->x) + tempX)] = '#';
                    wallsGenerated += 1;
                }
            }
            else 
            {
                if ((conditionHori1 && conditionHori2 && conditionHori3) && !(isPlayerPos && isEnemyPos && isExitPos))
                {
                    map[((tempY * map_size->x) + tempX)] = '#';
                    map[((tempY * map_size->x) + (tempX + 1))] = '#';
                    map[((tempY * map_size->x) + (tempX + 2))] = '#';
                    wallsGenerated += 1;
                }
            }
        } while (wallsGenerated < wallsToGenerate);
    } while(!isPlayerReachable(map, map_size, player, enemy) || !isExitReachable(map, map_size, player, enemy));
    printf("\nCOMPLETED WALL GENERATION!");
    // END OF WALLS

    // LOG FPRINTF
    FILE *logPtr = fopen("log.txt", "w");

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

    fprintf(logPtr, " ");
    for (int bW = 0; bW < borderOffsetX; bW++)
    {
        fprintf(logPtr, "%c", borderWidth[bW]);
    }
    fprintf(logPtr,"\n");
    for (int y = 0; y < map_size->y; y++) 
    {
        fprintf(logPtr,"%c", borderHeight[y]);
        for (int x = 0; x < map_size->x; x++)
        {
            // (y * width) + x = x, y 
            if (map[(y * map_size->x) + x] == player->icon)
            {
                fprintf(logPtr," %c ", map[(y * map_size->x) + x]);
            }
            else if (map[(y * map_size->x) + x] == enemy->icon)
            {
                fprintf(logPtr," %c ", map[(y * map_size->x) + x]);
            }
            else if (map[(y * map_size->x) + x] == '$')
            {
                fprintf(logPtr," %c ", map[(y * map_size->x) + x]);
            }
            else if (map[(y * map_size->x) + x] == '#')
            {
                fprintf(logPtr," %c ", map[(y * map_size->x) + x]);
            }
            else 
            {
                fprintf(logPtr," %c ", map[(y * map_size->x) + x]);
            }
        }
        fprintf(logPtr,"%c", borderHeight[y]);
        fprintf(logPtr,"\n");
    }
    fprintf(logPtr," ");
    for (int bW = 0; bW < borderOffsetX; bW++)
    {
        fprintf(logPtr,"%c", borderWidth[bW]);
    }
    fprintf(logPtr,"\n");

    fclose(logPtr);

}