#include "game_structures.h"
#include "movement_system.h"

// PathNode Struct

typedef struct 
{
    int g; // Cost from start to this tile
    int h; // Heuristic (Manhattan distance) to enemy/player
    int f; // Total cost (g + h)
    int parentX; // X coordinate of the tile we stepped here from
    int parentY; // Y coordinate of the tile we stepped here from

} PathNode;

// Worker functions

int manhattanDistance(int playerX, int enemyX, int playerY, int enemyY)
{
    return abs(playerX - enemyX) + abs(playerY - enemyY);
}

int backTrack(PathNode *pathGrid, Map *map_size, int currentX, int currentY, int startX, int startY)
{
    // Keep going back in nodes untill manhattan distance is equal to 1, and then we will just add 1 to the final counter
    int counter = 0;

    bool completed = false;

    int tempX = currentX; // start from current node
    int tempY = currentY;
    PathNode tempNode;

    int tempData = 0; // hold data

    do
    {
        tempData = tempX; // set tempData to tempX before we set it

        tempX = pathGrid[(tempY * map_size->x) + tempX].parentX;
        tempY = pathGrid[(tempY * map_size->x) + tempData].parentY;

        tempNode = pathGrid[(tempY * map_size->x) + tempX];

        if ((tempNode.parentX != startX) && (tempNode.parentY != startY))
        {
            counter += 1;
        }
        else
        {
            counter += 2; // add one for the node and then add another for the original position
            completed = true;
        }
    }while(!completed);
    
    return counter;
}

// End of Worker function

bool isExitReachable(char *map, Map *map_size, GameEntity *player, GameEntity *enemy) // find path from player to exit, and return true if found path. This will run each time map is generated
{
    int counter = 0;

    PathNode *pathGrid = malloc((map_size->x * map_size->y) * sizeof(PathNode));
    // pathGrid is where algorithm will be testing paths

    int currentX = player->position[0];
    int currentY = player->position[1];

    PathNode *tempLowestFVal1;
    PathNode *tempLowestFVal2;
    PathNode *lowestFVal;

    PathNode *up;
    PathNode *down;
    PathNode *left;
    PathNode *right;

    BotDirection tempDir;

    do
    {
        // map[(y * map_size->x) + x]

        // CHECK UP
        if (map[((currentY - 1) * map_size->x) + currentX] == ' ')
        {                                                                          //int goalX, int currentX, int goalY, int currentY
            pathGrid[((currentY - 1) * map_size->x) + currentX].h = manhattanDistance(exit_x, currentX, exit_y, currentY - 1);
                                                                                               //int currentX, int currentY - 1, int startX, int startY
            pathGrid[((currentY - 1) * map_size->x) + currentX].g = backTrack(pathGrid, map_size, currentX, currentY - 1, player->position[0], player->position[1]);

            pathGrid[((currentY - 1) * map_size->x) + currentX].f = (pathGrid[((currentY - 1) * map_size->x) + currentX].g) + (pathGrid[((currentY - 1) * map_size->x) + currentX].h);
        }
        // CHECK DOWN
        if (map[((currentY + 1) * map_size->x) + currentX] == ' ')
        {
            pathGrid[((currentY + 1) * map_size->x) + currentX].h = manhattanDistance(exit_x, currentX, exit_y, currentY + 1);
            pathGrid[((currentY + 1) * map_size->x) + currentX].g = backTrack(pathGrid, map_size, currentX, currentY + 1, player->position[0], player->position[1]);
            pathGrid[((currentY + 1) * map_size->x) + currentX].f = (pathGrid[((currentY + 1) * map_size->x) + currentX].g) + (pathGrid[((currentY + 1) * map_size->x) + currentX].h);
        }
        // CHECK LEFT
        if (map[(currentY * map_size->x) + (currentX - 1)] == ' ')
        {
            pathGrid[(currentY * map_size->x) + (currentX - 1)].h = manhattanDistance(exit_x, currentX - 1, exit_y, currentY);
            pathGrid[(currentY * map_size->x) + (currentX - 1)].g = backTrack(pathGrid, map_size, currentX - 1, currentY, player->position[0], player->position[1]);
            pathGrid[(currentY * map_size->x) + (currentX - 1)].f = (pathGrid[(currentY * map_size->x) + (currentX - 1)].g) + (pathGrid[(currentY * map_size->x) + (currentX - 1)].h);
        }
        // CHECK RIGHT
        if (map[(currentY * map_size->x) + (currentX + 1)] == ' ')
        {
            pathGrid[(currentY * map_size->x) + (currentX + 1)].h = manhattanDistance(exit_x, currentX + 1, exit_y, currentY);
            pathGrid[(currentY * map_size->x) + (currentX + 1)].g = backTrack(pathGrid, map_size, currentX + 1, currentY, player->position[0], player->position[1]);
            pathGrid[(currentY * map_size->x) + (currentX + 1)].f = (pathGrid[(currentY * map_size->x) + (currentX + 1)].g) + (pathGrid[(currentY * map_size->x) + (currentX + 1)].h);
        }   
        
        // WE CHECK ALL SURROUNDING NODES F AND CHOOSE LOWEST AND THEN CHOOSE UP, DOWN, RIGHT, LEFT

        up = &pathGrid[((currentY - 1) * map_size->x) + currentX];
        down = &pathGrid[((currentY + 1) * map_size->x) + currentX];
        left = &pathGrid[(currentY * map_size->x) + (currentX - 1)];
        right = &pathGrid[(currentY * map_size->x) + (currentX + 1)];

        tempLowestFVal1 = (up->f < down->f) ? up : down;
        tempLowestFVal2 = (left->f < right->f) ? left : right;
        lowestFVal = (tempLowestFVal1->f < tempLowestFVal2->f) ? tempLowestFVal1 : tempLowestFVal2;

        if (lowestFVal == up)
        {
            if (checkMove(map, map_size, player, enemy, 'W', 'P'))
            {
                tempDir = UP;
                currentY -= 1;
            }
        }
        else if (lowestFVal == down)
        {
            if (checkMove(map, map_size, player, enemy, 'S', 'P'))
            {
                tempDir = DOWN;
                currentY += 1;
            }
        }
        else if (lowestFVal == left)
        {
            if (checkMove(map, map_size, player, enemy, 'A', 'P'))
            {
                tempDir = LEFT;
                currentX -= 1;
            }
        }
        else if (lowestFVal == right)
        {
            if (checkMove(map, map_size, player, enemy, 'D', 'P'))
            {
                tempDir = RIGHT;
                currentX += 1;
            }
        }

        counter += 1;

        if (counter <= (map_size->x * map_size->y))
        {
            if (currentX == exit_x && currentY == exit_y)
            {
                return true; // path found
            }
        }
        else
        {
            return false; // Checked everywhere possible and could not find a path
        }

    } while (true); 
}

bool isPlayerReachable(char *map, Map *map_size, GameEntity *player, GameEntity *enemy) // This will run once when map is generated just to check if map generation allows enemy to reach player
{

    int counter = 0;

    PathNode *pathGrid = malloc((map_size->x * map_size->y) * sizeof(PathNode));
    // pathGrid is where algorithm will be testing paths

    int currentX = enemy->position[0];
    int currentY = enemy->position[1];

    PathNode *tempLowestFVal1;
    PathNode *tempLowestFVal2;
    PathNode *lowestFVal;

    PathNode *up;
    PathNode *down;
    PathNode *left;
    PathNode *right;

    BotDirection tempDir;

    do
    {
        // map[(y * map_size->x) + x]

        // CHECK UP
        if (map[((currentY - 1) * map_size->x) + currentX] == ' ')
        {
            pathGrid[((currentY - 1) * map_size->x) + currentX].h = manhattanDistance(player->position[0], currentX, player->position[1], currentY - 1);
            pathGrid[((currentY - 1) * map_size->x) + currentX].g = backTrack(pathGrid, map_size, currentX, currentY - 1, enemy->position[0], enemy->position[1]);
            pathGrid[((currentY - 1) * map_size->x) + currentX].f = (pathGrid[((currentY - 1) * map_size->x) + currentX].g) + (pathGrid[((currentY - 1) * map_size->x) + currentX].h);
        }
        // CHECK DOWN
        if (map[((currentY + 1) * map_size->x) + currentX] == ' ')
        {
            pathGrid[((currentY + 1) * map_size->x) + currentX].h = manhattanDistance(player->position[0], currentX, player->position[1], currentY + 1);
            pathGrid[((currentY + 1) * map_size->x) + currentX].g = backTrack(pathGrid, map_size, currentX, currentY + 1, enemy->position[0], enemy->position[1]);
            pathGrid[((currentY + 1) * map_size->x) + currentX].f = (pathGrid[((currentY + 1) * map_size->x) + currentX].g) + (pathGrid[((currentY + 1) * map_size->x) + currentX].h);
        }
        // CHECK LEFT
        if (map[(currentY * map_size->x) + (currentX - 1)] == ' ')
        {
            pathGrid[(currentY * map_size->x) + (currentX - 1)].h = manhattanDistance(player->position[0], currentX - 1, player->position[1], currentY);
            pathGrid[(currentY * map_size->x) + (currentX - 1)].g = backTrack(pathGrid, map_size, currentX - 1, currentY, enemy->position[0], enemy->position[1]);
            pathGrid[(currentY * map_size->x) + (currentX - 1)].f = (pathGrid[(currentY * map_size->x) + (currentX - 1)].g) + (pathGrid[(currentY * map_size->x) + (currentX - 1)].h);
        }
        // CHECK RIGHT
        if (map[(currentY * map_size->x) + (currentX + 1)] == ' ')
        {
            pathGrid[(currentY * map_size->x) + (currentX + 1)].h = manhattanDistance(player->position[0], currentX + 1, player->position[1], currentY);
            pathGrid[(currentY * map_size->x) + (currentX + 1)].g = backTrack(pathGrid, map_size, currentX + 1, currentY, enemy->position[0], enemy->position[1]);
            pathGrid[(currentY * map_size->x) + (currentX + 1)].f = (pathGrid[(currentY * map_size->x) + (currentX + 1)].g) + (pathGrid[(currentY * map_size->x) + (currentX + 1)].h);
        }   
        
        // WE CHECK ALL SURROUNDING NODES F AND CHOOSE LOWEST AND THEN CHOOSE UP, DOWN, RIGHT, LEFT

        up = &pathGrid[((currentY - 1) * map_size->x) + currentX];
        down = &pathGrid[((currentY + 1) * map_size->x) + currentX];
        left = &pathGrid[(currentY * map_size->x) + (currentX - 1)];
        right = &pathGrid[(currentY * map_size->x) + (currentX + 1)];

        tempLowestFVal1 = (up->f < down->f) ? up : down;
        tempLowestFVal2 = (left->f < right->f) ? left : right;
        lowestFVal = (tempLowestFVal1->f < tempLowestFVal2->f) ? tempLowestFVal1 : tempLowestFVal2;

        if (lowestFVal == up)
        {
            if (checkMove(map, map_size, player, enemy, 'W', 'E'))
            {
                tempDir = UP;
                currentY -= 1;
            }
        }
        else if (lowestFVal == down)
        {
            if (checkMove(map, map_size, player, enemy, 'S', 'E'))
            {
                tempDir = DOWN;
                currentY += 1;
            }
        }
        else if (lowestFVal == left)
        {
            if (checkMove(map, map_size, player, enemy, 'A', 'E'))
            {
                tempDir = LEFT;
                currentX -= 1;
            }
        }
        else if (lowestFVal == right)
        {
            if (checkMove(map, map_size, player, enemy, 'D', 'E'))
            {
                tempDir = RIGHT;
                currentX += 1;
            }
        }

        counter += 1;

        if (counter <= (map_size->x * map_size->y))
        {
            if (currentX == player->position[0] && currentY == player->position[1])
            {
                return true; // path found
            }
        }
        else
        {
            return false; // Checked everywhere possible and could not find a path
        }

    } while (true); 
}

void findPlayerPath(char *map, Map *map_size, GameEntity *player, GameEntity *enemy, BotDirection *botDirCache, size_t *botCacheSize) // find path from enemy to player, and set the botDirCache. This will run each time player MOVES, not if BLOCKED, and each time map is generated
{
    int counter = 0;

    bool pathFound = false;

    PathNode *pathGrid = malloc((map_size->x * map_size->y) * sizeof(PathNode));
    // pathGrid is where algorithm will be testing paths

    int currentX = enemy->position[0];
    int currentY = enemy->position[1];

    PathNode *tempLowestFVal1;
    PathNode *tempLowestFVal2;
    PathNode *lowestFVal;

    PathNode *up;
    PathNode *down;
    PathNode *left;
    PathNode *right;

    BotDirection tempDir;

    do
    {
        // map[(y * map_size->x) + x]

        // CHECK UP
        if (map[((currentY - 1) * map_size->x) + currentX] == ' ')
        {
            pathGrid[((currentY - 1) * map_size->x) + currentX].h = manhattanDistance(player->position[0], currentX, player->position[1], currentY - 1);
            pathGrid[((currentY - 1) * map_size->x) + currentX].g = backTrack(pathGrid, map_size, currentX, currentY - 1, enemy->position[0], enemy->position[1]);
            pathGrid[((currentY - 1) * map_size->x) + currentX].f = (pathGrid[((currentY - 1) * map_size->x) + currentX].g) + (pathGrid[((currentY - 1) * map_size->x) + currentX].h);
        }
        // CHECK DOWN
        if (map[((currentY + 1) * map_size->x) + currentX] == ' ')
        {
            pathGrid[((currentY + 1) * map_size->x) + currentX].h = manhattanDistance(player->position[0], currentX, player->position[1], currentY + 1);
            pathGrid[((currentY + 1) * map_size->x) + currentX].g = backTrack(pathGrid, map_size, currentX, currentY + 1, enemy->position[0], enemy->position[1]);
            pathGrid[((currentY + 1) * map_size->x) + currentX].f = (pathGrid[((currentY + 1) * map_size->x) + currentX].g) + (pathGrid[((currentY + 1) * map_size->x) + currentX].h);
        }
        // CHECK LEFT
        if (map[(currentY * map_size->x) + (currentX - 1)] == ' ')
        {
            pathGrid[(currentY * map_size->x) + (currentX - 1)].h = manhattanDistance(player->position[0], currentX - 1, player->position[1], currentY);
            pathGrid[(currentY * map_size->x) + (currentX - 1)].g = backTrack(pathGrid, map_size, currentX - 1, currentY, enemy->position[0], enemy->position[1]);
            pathGrid[(currentY * map_size->x) + (currentX - 1)].f = (pathGrid[(currentY * map_size->x) + (currentX - 1)].g) + (pathGrid[(currentY * map_size->x) + (currentX - 1)].h);
        }
        // CHECK RIGHT
        if (map[(currentY * map_size->x) + (currentX + 1)] == ' ')
        {
            pathGrid[(currentY * map_size->x) + (currentX + 1)].h = manhattanDistance(player->position[0], currentX + 1, player->position[1], currentY);
            pathGrid[(currentY * map_size->x) + (currentX + 1)].g = backTrack(pathGrid, map_size, currentX + 1, currentY, enemy->position[0], enemy->position[1]);
            pathGrid[(currentY * map_size->x) + (currentX + 1)].f = (pathGrid[(currentY * map_size->x) + (currentX + 1)].g) + (pathGrid[(currentY * map_size->x) + (currentX + 1)].h);
        }   
        
        // WE CHECK ALL SURROUNDING NODES F AND CHOOSE LOWEST AND THEN CHOOSE UP, DOWN, RIGHT, LEFT

        up = &pathGrid[((currentY - 1) * map_size->x) + currentX];
        down = &pathGrid[((currentY + 1) * map_size->x) + currentX];
        left = &pathGrid[(currentY * map_size->x) + (currentX - 1)];
        right = &pathGrid[(currentY * map_size->x) + (currentX + 1)];

        tempLowestFVal1 = (up->f < down->f) ? up : down;
        tempLowestFVal2 = (left->f < right->f) ? left : right;
        lowestFVal = (tempLowestFVal1->f < tempLowestFVal2->f) ? tempLowestFVal1 : tempLowestFVal2;

        if (lowestFVal == up)
        {
            if (checkMove(map, map_size, player, enemy, 'W', 'E'))
            {
                tempDir = UP;
                currentY -= 1;
            }
        }
        else if (lowestFVal == down)
        {
            if (checkMove(map, map_size, player, enemy, 'S', 'E'))
            {
                tempDir = DOWN;
                currentY += 1;
            }
        }
        else if (lowestFVal == left)
        {
            if (checkMove(map, map_size, player, enemy, 'A', 'E'))
            {
                tempDir = LEFT;
                currentX -= 1;
            }
        }
        else if (lowestFVal == right)
        {
            if (checkMove(map, map_size, player, enemy, 'D', 'E'))
            {
                tempDir = RIGHT;
                currentX += 1;
            }
        }

        botDirCache[counter] = tempDir;
        counter += 1;

        if (currentX == player->position[0] && currentY == player->position[1]) // We do not need same logic as isPlayerReachable because
        {                                                                       // This function just finds a new path when player moves, while the isPlayerReachable
            pathFound = true; // path found                                          Function checks if when the map is generated if the player is reachable with map configuration
        }                                                                    

    } while (!pathFound); // If completed we will set botDirCache to the 
                          // path translated to W A S D 

    // REALLOCATE BOTDIRCACHE only as big as the amount of directions we have inside

    //size_t botCacheSize = map_size.x * map_size.y; // 100 possible moves for 100 spaces in array
    //BotDirection *botDirCache = malloc(botCacheSize * sizeof(BotDirection));

    *botCacheSize = (size_t)counter; // Items in bot cache, cast int to size_t
    BotDirection *temp = realloc(botDirCache, (*botCacheSize * sizeof(BotDirection)));

    if (temp == NULL)
    {
        printf("\nMemory reallocation Failed.\n");
        free(botDirCache);
        botDirCache = NULL;
        exit(1);
    }
    
    botDirCache = temp;
    temp = NULL;

}