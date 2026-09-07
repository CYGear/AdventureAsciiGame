// MY HEADER FILES:
#include "map_generation.h"
#include "draw_map.h"
#include "movement_system.h"
#include "update.h"
#include "pathfinding.h"

int exit_x = 0;
int exit_y = 0;

int level = 1;

int playerBlockedCounter = 0;

void initializeGame(GameEntity *player, GameEntity *enemy)
{
    bool completed = false;

    char confirmation = ' '; 

    do
    {
        clearTerminal();

        printf("\nEnter your character(CANNOT BE !, #, OR $): ");
        scanf(" %c", &(player->icon)); // address of icon inside player

        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        
        if (player->icon != enemy->icon && player->icon != '$' && player->icon != '#') // de-reference player and then access icon variable in struct
        {
            printf("Character selected: %c\nConfirm? (y/n)> ", player->icon);
            scanf(" %c", &confirmation);

            switch (tolower(confirmation))
            {
                case 'y': completed = true; clearTerminal(); break;
                case 'n': break;
                default: printf("\nInvalid Input...");
            }
        }

    }while(!completed); 
}

void increaseMapSize(char **map, Map *map_size, BotDirection **botDirCache, size_t *botCacheSize) // I need ** to actually chnage memory address. * in a
{                                               // function only copies the memory address it can't actually change it

    map_size->x += 2;
    map_size->y += 2;

    *botCacheSize = (map_size->x * map_size->y);

    char *temp1 = realloc(*map, (map_size->x * map_size->y) * sizeof(char));
    if (temp1 == NULL)
    {
        printf("Could not reallocate memory.\n");
        free(*map);
        exit(1);
    }
    else
    {
        *map = temp1;
        temp1 = NULL; // prevent dangling ptr
    }

    BotDirection *temp2 = realloc(*botDirCache, (*botCacheSize * sizeof(BotDirection)));
    if (temp2 == NULL)
    {
        printf("Could not reallocate memory.\n");
        free(*botDirCache);
        exit(1);
    }
    else
    {
        *botDirCache = temp2;
        temp2 = NULL; // prevent dangling ptr
    }

}

int main()
{
    // I need 2d character array for spots
    // # = walls, player = anything player inputs at beginning(EXCEPT ! and #), enemy = !, exit = $

    // MAP ARRAY: Dynamic map array using first a size of 10 * 10 map of characters so we need to allocate
    //            100 bytes.

    //       Cols              rows
    Map map_size; // 10 * 10 map
    map_size.x = 10;
    map_size.y = 10;

    char *map = malloc(map_size.x * map_size.y * sizeof(char)); // allocates 100 bytes

    size_t botCacheSize = map_size.x * map_size.y; // 100 possible moves for 100 spaces in array
    BotDirection *botDirCache = malloc(botCacheSize * sizeof(BotDirection));

    // Use realloc later on for map size change

    // For character and enemy we will use GameEntity structs to keep track of character icon and position on map grid  

    GameEntity player;
    GameEntity enemy;
    enemy.icon = '!';
    // set x and y to complete opposite side of player. Instead of 0, 0 it would be 9, 9 for start map
    enemy.position[0] = 9; // because of how 2d arrays work, it would be first y and then x, but for struct pos we do not inforce that
    enemy.position[1] = 9;

    player.position[0] = 0; // x
    player.position[1] = 0; // y

    // char exit = '$';
    // char wall = '#';

    initializeGame(&player, &enemy);
    generateMap(map, &map_size, &player, &enemy, isPlayerReachable, isExitReachable, botDirCache, &botCacheSize); // will be called when exit 
    MoveStatus result = VALID;

    while (true)
    {
        result = inputLoop(map, &map_size, &player, &enemy, updateMap, drawMap, findPlayerPath);

        switch (result)
        {
            case EXITED: // Resize map and re-locate all GameEntities
                clearTerminal();
                printf("\nINCREASING MAP SIZE NOW!");
                increaseMapSize(&map, &map_size, &botDirCache, &botCacheSize);
                generateMap(map, &map_size, &player, &enemy, isPlayerReachable, isExitReachable, botDirCache, &botCacheSize);
                level += 1;
                break;

            case DEAD: clearTerminal(); exit(0);
        }

    }

    // free map, adn botDirCache memory
    free(map);
    map = NULL;
    
    free(botDirCache);
    botDirCache = NULL;

    return 0;
}