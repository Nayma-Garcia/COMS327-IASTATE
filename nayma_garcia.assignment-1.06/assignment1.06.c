#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <limits.h> 
#include <string.h>
#include <ncurses.h>


#define NUM_MAPSX 401
#define NUM_MAPSY 401
#define LENGTH 21
#define WIDTH 80
#define GRASS_PAIR 1
#define TREE_PAIR 2
#define WATER_PAIR 3
#define BOULDER_PAIR 4
#define PLAYER_PAIR 5
#define POKECENTER 6
#define POKEMART 7
#define PATH 8

typedef struct {
    char terrainChar;
    int minRegions;
    int maxRegions;
    int minWidth;
    int maxWidth;
    int minHeight;
    int maxHeight;
} TerrainType;

TerrainType terrainTypes[] = {
    {':', 2, 10, 4, 13, 4, 9},  // tall grass
    {'^', 2, 6, 4, 13, 4, 9},   // trees
    {'~', 1, 5, 4, 13, 4, 9},   // water
    {'%', 1, 5, 4, 13, 4, 9},   // boulders
};

// Define a structure to represent a map
typedef struct {
    char map[LENGTH][WIDTH];
} Map;

typedef struct {
    int x;
    int y;
} Coordinate;

typedef struct {
    char symbol;
    Coordinate position;
} Hiker;

Hiker hiker;

typedef struct {
    char symbol;
    Coordinate position;
} PC;

PC pc;

typedef struct {
    char symbol;
    Coordinate position;
} Rival;

Rival rival;

typedef struct {
    char symbol;
    Coordinate position;
    int direction; 
} Pacer;

Pacer pacer;

typedef struct {
    char symbol;
    Coordinate position;
    int direction; 
} Wanderer;

Wanderer wanderer;

typedef struct {
    char symbol;
    Coordinate position;
} Sentrie;

Sentrie sentrie;

typedef struct {
    char symbol;
    Coordinate position;
    int direction; 
} Explorer;

Explorer explorer;

typedef struct {
    char symbol;
    Coordinate position;
    int direction; 
} PokeCenter;

PokeCenter pokeCenter;

typedef struct {
    char symbol;
    Coordinate position;
    int direction; 
} PokeMart;

PokeMart pokeMart;

int hikerDirection = 0; 
int rivalDirection = 0; 

int hasHikerBeenBattled = 0;
int hasRivalBeenBattled = 0;
int hasPacerBeenBattled = 0;
int hasWandererBeenBattled = 0;
int hasSentrieBeenBattled = 0;
int hasExplorerBeenBattled = 0;

/*-----FUNCTION PROTOTYPES---------*/
void generateMap(Map *map);
void printMap(Map *map);
void fillMapGrass(Map *map);
void generateBorder(Map *map);
void genPathCM(Map *map, char direction);
void generateTerrain(Map *map);
int getRandom(int min, int max);
int isPathThere(char symbol, int x, int y, int width, int height, Map *map);
int isCMOffMap(int x, int y, Map *map);
void updateHikerLocation(Map *map);
void updateRivalLocation(Map *map);
void updatePacerLocation(Map *map);
void updateWandererLocation(Map *map);
void updateExplorerLocation(Map *map);
void updatePCLocation(Map *map, int move);
void openTrainerLocationList(Map *map);
void hikerPosFromPC(int hikerX, int hikerY, int pcX, int pcY);
void rivalPosFromPC(int rivalX, int rivalY, int pcX, int pcY);
void pacerPosFromPC(int pacerX, int pacerY, int pcX, int pcY);
void wandererPosFromPC(int wandererX, int wandererY, int pcX, int pcY);
void sentriePosFromPC(int sentrieX, int sentrieY, int pcX, int pcY);
void explorerPosFromPC(int explorerX, int explorerY, int pcX, int pcY);
int isPConCorM(Map *map);
void isTrainerThere();
void isHikerThere();
void isRivalThere();
void isPacerThere();
void isWandererThere();
void isSentrieThere();
void isExplorerThere();
void  clearMapAroundTrainerList();

//global variables
char currentDirection;

int main(int argc, char* argv[]) {

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    start_color();
    init_pair(GRASS_PAIR, COLOR_GREEN, COLOR_BLACK);
    init_pair(WATER_PAIR, COLOR_BLUE, COLOR_BLACK);
    init_pair(BOULDER_PAIR, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(TREE_PAIR, COLOR_YELLOW, COLOR_BLACK);
    init_pair(PLAYER_PAIR, COLOR_RED, COLOR_BLACK);
    init_pair(POKECENTER, COLOR_CYAN, COLOR_BLACK);
    init_pair(POKEMART, COLOR_CYAN, COLOR_BLACK);
    init_pair(PATH, COLOR_YELLOW, COLOR_BLACK);

    srand(time(NULL)); 

    int currentMapX = 200;
    int currentMapY = 200;
    int pathGenerated[NUM_MAPSX][NUM_MAPSY] = {0};

    Map** maps;
    maps = (Map**)malloc(NUM_MAPSX * sizeof(Map*));
    for (int x = 0; x < NUM_MAPSX; x++) {
        maps[x] = (Map*)malloc(NUM_MAPSY * sizeof(Map));
    }

    for (int x = 0; x < NUM_MAPSX; x++) {
        for (int y = 0; y < NUM_MAPSY; y++) {
            generateMap(&maps[x][y]);
        }
    }

    generateMap(&maps[currentMapX][currentMapY]);
    int move;

     printMap(&maps[currentMapX][currentMapY]);
     mvprintw(22, 0, "Please enter a command");
    if(pathGenerated[currentMapX][currentMapY] == 0){
        genPathCM(&maps[currentMapX][currentMapY], currentDirection);
        pathGenerated[currentMapX][currentMapY] = 1;
    }

    while ((move = getch()) != 'q') {
         isTrainerThere();
        if(move == '7' || move == 'y'){
            updatePCLocation(&maps[currentMapX][currentMapY], move);
            updateHikerLocation(&maps[currentMapX][currentMapY]);
            updateRivalLocation(&maps[currentMapX][currentMapY]);
            updatePacerLocation(&maps[currentMapX][currentMapY]);
            updateWandererLocation(&maps[currentMapX][currentMapY]);
            updateExplorerLocation(&maps[currentMapX][currentMapY]);
            fflush(stdout);
            usleep(50000);
            printMap(&maps[currentMapX][currentMapY]);
        }else if(move == '8' || move == 'k'){
        updatePCLocation(&maps[currentMapX][currentMapY], move);
            updateHikerLocation(&maps[currentMapX][currentMapY]);
            updateRivalLocation(&maps[currentMapX][currentMapY]);
            updatePacerLocation(&maps[currentMapX][currentMapY]);
            updateWandererLocation(&maps[currentMapX][currentMapY]);
            updateExplorerLocation(&maps[currentMapX][currentMapY]);
            fflush(stdout);
            usleep(50000);
            printMap(&maps[currentMapX][currentMapY]);
       }else if(move == '9' || move == 'u'){
       updatePCLocation(&maps[currentMapX][currentMapY], move);
            updateHikerLocation(&maps[currentMapX][currentMapY]);
            updateRivalLocation(&maps[currentMapX][currentMapY]);
            updatePacerLocation(&maps[currentMapX][currentMapY]);
            updateWandererLocation(&maps[currentMapX][currentMapY]);
            updateExplorerLocation(&maps[currentMapX][currentMapY]);
            fflush(stdout);
            usleep(50000);
            printMap(&maps[currentMapX][currentMapY]);
       }else if(move =='6' || move == 'l'){
        updatePCLocation(&maps[currentMapX][currentMapY], move);
            updateHikerLocation(&maps[currentMapX][currentMapY]);
            updateRivalLocation(&maps[currentMapX][currentMapY]);
            updatePacerLocation(&maps[currentMapX][currentMapY]);
            updateWandererLocation(&maps[currentMapX][currentMapY]);
            updateExplorerLocation(&maps[currentMapX][currentMapY]);
            fflush(stdout);
            usleep(50000);
            printMap(&maps[currentMapX][currentMapY]);
       }else if(move =='3' || move == 'n'){
       updatePCLocation(&maps[currentMapX][currentMapY], move);
            updateHikerLocation(&maps[currentMapX][currentMapY]);
            updateRivalLocation(&maps[currentMapX][currentMapY]);
            updatePacerLocation(&maps[currentMapX][currentMapY]);
            updateWandererLocation(&maps[currentMapX][currentMapY]);
            updateExplorerLocation(&maps[currentMapX][currentMapY]);
            fflush(stdout);
            usleep(50000);
            printMap(&maps[currentMapX][currentMapY]);
       }else if(move =='2' || move == 'j'){
        updatePCLocation(&maps[currentMapX][currentMapY], move);
            updateHikerLocation(&maps[currentMapX][currentMapY]);
            updateRivalLocation(&maps[currentMapX][currentMapY]);
            updatePacerLocation(&maps[currentMapX][currentMapY]);
            updateWandererLocation(&maps[currentMapX][currentMapY]);
            updateExplorerLocation(&maps[currentMapX][currentMapY]);
            fflush(stdout);
            usleep(50000);
            printMap(&maps[currentMapX][currentMapY]);
       }else if(move =='1' || move == 'b'){
        updatePCLocation(&maps[currentMapX][currentMapY], move);
            updateHikerLocation(&maps[currentMapX][currentMapY]);
            updateRivalLocation(&maps[currentMapX][currentMapY]);
            updatePacerLocation(&maps[currentMapX][currentMapY]);
            updateWandererLocation(&maps[currentMapX][currentMapY]);
            updateExplorerLocation(&maps[currentMapX][currentMapY]);
            fflush(stdout);
            usleep(50000);
            printMap(&maps[currentMapX][currentMapY]);
       }else if(move =='4' || move == 'h'){
            updatePCLocation(&maps[currentMapX][currentMapY], move);
            updateHikerLocation(&maps[currentMapX][currentMapY]);
            updateRivalLocation(&maps[currentMapX][currentMapY]);
            updatePacerLocation(&maps[currentMapX][currentMapY]);
            updateWandererLocation(&maps[currentMapX][currentMapY]);
            updateExplorerLocation(&maps[currentMapX][currentMapY]);
            fflush(stdout);
            usleep(50000);
            printMap(&maps[currentMapX][currentMapY]);
       }else if(move == '5' || move =='.' || move == ' '){
            updatePCLocation(&maps[currentMapX][currentMapY], move);
            updateHikerLocation(&maps[currentMapX][currentMapY]);
            updateRivalLocation(&maps[currentMapX][currentMapY]);
            updatePacerLocation(&maps[currentMapX][currentMapY]);
            updateWandererLocation(&maps[currentMapX][currentMapY]);
            updateExplorerLocation(&maps[currentMapX][currentMapY]);
            fflush(stdout);
            usleep(50000);
            printMap(&maps[currentMapX][currentMapY]);
       }else if(move =='t'){
          fflush(stdout);
          usleep(50000);
          printMap(&maps[currentMapX][currentMapY]);

          clearMapAroundTrainerList();
          //hiker
          hikerPosFromPC(hiker.position.x, hiker.position.y,pc.position.x, pc.position.y);
          //rival
          rivalPosFromPC(rival.position.x, rival.position.y,pc.position.x, pc.position.y);
          //pacer
          pacerPosFromPC(pacer.position.x, pacer.position.y,pc.position.x, pc.position.y);
          //wanderer
          wandererPosFromPC(wanderer.position.x, wanderer.position.y,pc.position.x, pc.position.y);
          //sentrie
          sentriePosFromPC(sentrie.position.x, sentrie.position.y,pc.position.x, pc.position.y);
          //explorer
          explorerPosFromPC(explorer.position.x, explorer.position.y,pc.position.x, pc.position.y);
       }else if(move == '>' && isPConCorM(&maps[currentMapX][currentMapY]) == 1){
         mvprintw(22, 0, "Welcome to the PokeCenter!");
       }else if (move == '>' && isPConCorM(&maps[currentMapX][currentMapY]) == 2){
        mvprintw(22, 0, "Welcome to the PokeMart!");
       }else if(move == '<' || move == 27){
            updatePCLocation(&maps[currentMapX][currentMapY], move);
            fflush(stdout);
            usleep(500000);
            printMap(&maps[currentMapX][currentMapY]);
       }

      
    }
 
    // while (1) {
    //     printMap(&maps[currentMapX][currentMapY]);
    //     printf("Current Map Coordinates: (%d, %d):\n", currentMapX, currentMapY);
    //      fflush(stdout);
        
    //     printf("Enter 'n' to move north, 's' to move south, 'w' to move west, 'e' to move east, or 'q' to quit: ");
    //     char input;
    //     scanf(" %c", &input);
        
    //     if (input == 'n' && currentMapY > 0) {
    //         currentMapY--;
    //         currentDirection = 'n';
    //          if(pathGenerated[currentMapX][currentMapY] == 0){
    //             genPathCM(&maps[currentMapX][currentMapY], currentDirection);
    //             pathGenerated[currentMapX][currentMapY] = 1;
    //         }
    //     } else if (input == 's' && currentMapY < NUM_MAPSY - 1) {
    //         currentMapY++;
    //         currentDirection = 's';
    //        if(pathGenerated[currentMapX][currentMapY] == 0){
    //             genPathCM(&maps[currentMapX][currentMapY], currentDirection);
    //             pathGenerated[currentMapX][currentMapY] = 1;
    //         }
    //     } else if (input == 'w' && currentMapX > 0) {
    //         currentMapX--;
    //        currentDirection = 'w';
    //       if(pathGenerated[currentMapX][currentMapY] == 0){
    //             genPathCM(&maps[currentMapX][currentMapY], currentDirection);
    //             pathGenerated[currentMapX][currentMapY] = 1;
    //         }
    //     } else if (input == 'e' && currentMapX < NUM_MAPSX - 1) {
    //        currentMapX++;
    //        currentDirection = 'e';
    //        if(pathGenerated[currentMapX][currentMapY] == 0){
    //             genPathCM(&maps[currentMapX][currentMapY], currentDirection);
    //             pathGenerated[currentMapX][currentMapY] = 1;
    //         }
    //     } else if(input == 'f'){
    //         int inputX;
    //         int inputY;
    //         printf("enter an x and y coordinate separated by a comma: ");
    //         scanf("%d,%d", &inputX, &inputY);
    //         if (inputX >= 0 && inputX < NUM_MAPSX && inputY >= 0 && inputY < NUM_MAPSY) {
    //             currentMapX = inputX;
    //             currentMapY = inputY;
    //            if(pathGenerated[currentMapX][currentMapY] == 0){
    //             genPathCM(&maps[currentMapX][currentMapY], currentDirection);
    //             pathGenerated[currentMapX][currentMapY] = 1;
    //         }
    //         } else {
    //             printf("Invalid coordinates. Try again.\n");
    //         }
    //     } else if (input == 'q') {
    //         break;
    //     } else {
    //         printf("Invalid input. Try again.\n");
    //     }
    //     printf("current direction: %c\n", currentDirection);
    // }
    for (int x = 0; x < NUM_MAPSX; x++) {
        free(maps[x]);
    }
    free(maps);
    return 0;
}

void generateMap(Map *map) {
    fillMapGrass(map);
    generateBorder(map);
    // genPathCM(map, direction);
    generateTerrain(map);
}

void printMap(Map *map) {
    int x;
    int y;
    for (y = 0; y < LENGTH; y++) {
        for (x = 0; x < WIDTH; x++) {
            printf("%c", map->map[y][x]);
        }
        printf("\n");
    }
}

void fillMapGrass(Map *map) {
    int x, y;
    for (y = 0; y < LENGTH; y++) {
        for (x = 0; x < WIDTH; x++) {
            map->map[y][x] = '.';
        }
    }
}

void generateBorder(Map *map) {
    int x, y;
    for (x = 0; x < WIDTH; x++) {
        map->map[0][x] = '%'; //starting at the left corner
        map->map[LENGTH - 1][x] = '%';
    }
    for (y = 0; y < LENGTH; y++) {
        map->map[y][0] = '%';
        map->map[y][WIDTH - 1] = '%';
    }
}

int lastVerticalPathX = -1;
int lastHorizontalPathY = -1;

void genPathCM(Map *map, char direction) {
    // Making the path that goes from east to west
    char testdir = direction;
    int x, y;
    y = rand() % (LENGTH - 3) + 2;  // So it doesn't touch the top border

if(testdir  == 'w' || testdir == 'e'){
    if (lastHorizontalPathY != -1) {
        y = lastHorizontalPathY;
    }
}
    for (x = 0; x < WIDTH; x++) {
        map->map[y][x] = '#';
    }

    lastHorizontalPathY = y;

    // Place the C in a random place right next to the path
    x = rand() % (WIDTH - 3);

    if (y < 21 && y > 2 && x < 80 && x > 2) {
        map->map[y + 1][x] = 'C';
    } else {
        map->map[y - 1][x] = 'C';
    }

    // Place the M in a random place right next to the path
    x = rand() % (WIDTH - 3);

    if (x < 80 && x > 2 && y < 21 && y > 2) {
        map->map[y + 1][x] = 'M';
    } else if (map->map[y][x] == '#') {
        map->map[y][x + 1] = 'M';
    } else {
        map->map[y - 1][x] = 'M';
    }

    x = rand() % (WIDTH - 3) + 2;

    // Use the last vertical path position from the previous map
    if(direction == 'n' || direction == 's'){
        if (lastVerticalPathX != -1) {
            x = lastVerticalPathX;
        }
    }
    for (y = 0; y < LENGTH; y++) {
        map->map[y][x] = '#';
    }

    // Update the last vertical path position
    lastVerticalPathX = x;
}


void generateTerrain(Map *map) {
    int numTerrainTypes = sizeof(terrainTypes) / sizeof(terrainTypes[0]);

    for (int i = 0; i < numTerrainTypes; i++) {
        TerrainType terrainType = terrainTypes[i];
        int numRegions = getRandom(terrainType.minRegions, terrainType.maxRegions);

        for (int j = 0; j < numRegions; j++) {
            int regionWidth = getRandom(terrainType.minWidth, terrainType.maxWidth);
            int regionHeight = getRandom(terrainType.minHeight, terrainType.maxHeight);
            int x, y;

            do {
                x = getRandom(1, WIDTH - regionWidth - 1);
                y = getRandom(1, LENGTH - regionHeight - 1);
            } while (isPathThere('#', x, y, regionWidth, regionHeight, map));

            for (int ty = y; ty < y + regionHeight; ty++) {
                for (int tx = x; tx < x + regionWidth; tx++) {
                    map->map[ty][tx] = terrainType.terrainChar;
                }
            }
        }
    }
}

int getRandom(int min, int max) {
    return min + rand() % (max - min + 1);
}

int isPathThere(char symbol, int x, int y, int width, int height, Map *map) {
    for (int ty = y; ty < y + height; ty++) {
        for (int tx = x; tx < x + width; tx++) {
            if (map->map[ty][tx] == symbol) {
                return 1;
            }
        }
    }
    return 0;
}

void updateHikerLocation(Map *map) {
    // Calculate the direction to move towards the PC
    int dx = pc.position.x - hiker.position.x;
    int dy = pc.position.y - hiker.position.y;

    int newX = hiker.position.x;
    int newY = hiker.position.y;

    if (dx > 0 && map->map[hiker.position.y][hiker.position.x + 1] != '@' && dx > 0 && map->map[hiker.position.y][hiker.position.x + 1] != '%') {
        newX++;
    } else if (dx < 0 && map->map[hiker.position.y][hiker.position.x - 1] != '@' && dx < 0 && map->map[hiker.position.y][hiker.position.x - 1] != '%') {
        newX--;
    } else if (dy > 0 && map->map[hiker.position.y + 1][hiker.position.x] != '@' && dy > 0 && map->map[hiker.position.y + 1][hiker.position.x] != '%') {
        newY++;
    } else if (dy < 0 && map->map[hiker.position.y - 1][hiker.position.x] != '@' && dy < 0 && map->map[hiker.position.y - 1][hiker.position.x] != '%') {
        newY--;
    }

    if (newX == pc.position.x && newY == pc.position.y) {
        int alternateDirection = rand() % 4;
        switch (alternateDirection) {
            case 0:
                if (map->map[hiker.position.y][hiker.position.x + 1] != '@' && map->map[hiker.position.y][hiker.position.x + 1] != '%') {
                    newX++;
                }
                break;
            case 1:
                if (map->map[hiker.position.y][hiker.position.x - 1] != '@' && map->map[hiker.position.y][hiker.position.x - 1] != '%') {
                    newX--;
                }
                break;
            case 2:
                if (map->map[hiker.position.y + 1][hiker.position.x] != '@' && map->map[hiker.position.y + 1][hiker.position.x] != '%') {
                    newY++;
                }
                break;
            case 3:
                if (map->map[hiker.position.y - 1][hiker.position.x] != '@' && map->map[hiker.position.y - 1][hiker.position.x] != '%') {
                    newY--;
                }
                break;
        }
    }

    // Ensure the new position is within the map boundaries
    if (newX >= 0 && newX < WIDTH && newY >= 0 && newY < LENGTH) {
        hiker.position.x = newX;
        hiker.position.y = newY;
    }
}

void updateRivalLocation(Map *map) {
    int dx = pc.position.x - rival.position.x;
    int dy = pc.position.y - rival.position.y;

    int newX = rival.position.x;
    int newY = rival.position.y;

    if (dx > 0 && map->map[rival.position.y][rival.position.x + 1] != '@' && map->map[rival.position.y][rival.position.x + 1] != '%' &&
        map->map[rival.position.y][rival.position.x + 1] != 'h' && map->map[rival.position.y][rival.position.x + 1] != 'p' &&
        map->map[rival.position.y][rival.position.x + 1] != 'w' && map->map[rival.position.y][rival.position.x + 1] != 's' &&
        map->map[rival.position.y][rival.position.x + 1] != 'r') {
        newX++;
    } else if (dx < 0 && map->map[rival.position.y][rival.position.x - 1] != '@' && map->map[rival.position.y][rival.position.x - 1] != '%' &&
               map->map[rival.position.y][rival.position.x - 1] != 'h' && map->map[rival.position.y][rival.position.x - 1] != 'p' &&
               map->map[rival.position.y][rival.position.x - 1] != 'w' && map->map[rival.position.y][rival.position.x - 1] != 's' &&
               map->map[rival.position.y][rival.position.x - 1] != 'r') {
        newX--;
    } else if (dy > 0 && map->map[rival.position.y + 1][rival.position.x] != '@' && map->map[rival.position.y + 1][rival.position.x] != '%' &&
               map->map[rival.position.y + 1][rival.position.x] != 'h' && map->map[rival.position.y + 1][rival.position.x] != 'p' &&
               map->map[rival.position.y + 1][rival.position.x] != 'w' && map->map[rival.position.y + 1][rival.position.x] != 's' &&
               map->map[rival.position.y + 1][rival.position.x] != 'r') {
        newY++;
    } else if (dy < 0 && map->map[rival.position.y - 1][rival.position.x] != '@' && map->map[rival.position.y - 1][rival.position.x] != '%' &&
               map->map[rival.position.y - 1][rival.position.x] != 'h' && map->map[rival.position.y - 1][rival.position.x] != 'p' &&
               map->map[rival.position.y - 1][rival.position.x] != 'w' && map->map[rival.position.y - 1][rival.position.x] != 's' &&
               map->map[rival.position.y - 1][rival.position.x] != 'r') {
        newY--;
    }

    if (newX == pc.position.x && newY == pc.position.y) {
        int alternateDirection = rand() % 4;
        switch (alternateDirection) {
            case 0:
                if (map->map[rival.position.y][rival.position.x + 1] != '@' && map->map[rival.position.y][rival.position.x + 1] != '%' &&
                    map->map[rival.position.y][rival.position.x + 1] != 'h' && map->map[rival.position.y][rival.position.x + 1] != 'p' &&
                    map->map[rival.position.y][rival.position.x + 1] != 'w' && map->map[rival.position.y][rival.position.x + 1] != 's' &&
                    map->map[rival.position.y][rival.position.x + 1] != 'r') {
                    newX++;
                }
                break;
            case 1:
                if (map->map[rival.position.y][rival.position.x - 1] != '@' && map->map[rival.position.y][rival.position.x - 1] != '%' &&
                    map->map[rival.position.y][rival.position.x - 1] != 'h' && map->map[rival.position.y][rival.position.x - 1] != 'p' &&
                    map->map[rival.position.y][rival.position.x - 1] != 'w' && map->map[rival.position.y][rival.position.x - 1] != 's' &&
                    map->map[rival.position.y][rival.position.x - 1] != 'r') {
                    newX--;
                }
                break;
            case 2:
                if (map->map[rival.position.y + 1][rival.position.x] != '@' && map->map[rival.position.y + 1][rival.position.x] != '%' &&
                    map->map[rival.position.y + 1][rival.position.x] != 'h' && map->map[rival.position.y + 1][rival.position.x] != 'p' &&
                    map->map[rival.position.y + 1][rival.position.x] != 'w' && map->map[rival.position.y + 1][rival.position.x] != 's' &&
                    map->map[rival.position.y + 1][rival.position.x] != 'r') {
                    newY++;
                }
                break;
            case 3:
                if (map->map[rival.position.y - 1][rival.position.x] != '@' && map->map[rival.position.y - 1][rival.position.x] != '%' &&
                    map->map[rival.position.y - 1][rival.position.x] != 'h' && map->map[rival.position.y - 1][rival.position.x] != 'p' &&
                    map->map[rival.position.y - 1][rival.position.x] != 'w' && map->map[rival.position.y - 1][rival.position.x] != 's' &&
                    map->map[rival.position.y - 1][rival.position.x] != 'r') {
                    newY--;
                }
                break;
        }
    }

    if (newX >= 0 && newX < WIDTH && newY >= 0 && newY < LENGTH) {
        rival.position.x = newX;
        rival.position.y = newY;
    }
}




void updatePacerLocation(Map *map) {
    int newX = pacer.position.x;
    int newY = pacer.position.y;

    if (pacer.direction == 0) { 
        newX++;
    } else if (pacer.direction == 1) { 
        newX--;
    } else if (pacer.direction == 2) { 
        newY--;
    } else if (pacer.direction == 3) { 
        newY++;
    }

    char nextTerrain = map->map[newY][newX];

    if (!(newX >= WIDTH - 1 || newX <= 0 || newY >= LENGTH - 1 || newY <= 0) &&
        (nextTerrain != '%' && nextTerrain != '^' && nextTerrain != '~')) {
        pacer.position.x = newX;
        pacer.position.y = newY;
    } else {
        if (pacer.direction == 0) {
            pacer.direction = 1; 
        } else if (pacer.direction == 1) {
            pacer.direction = 0; 
        } else if (pacer.direction == 2) {
            pacer.direction = 3;
        } else if (pacer.direction == 3) {
            pacer.direction = 2; 
        }
    }
}




void updateWandererLocation(Map *map) {
    int newX = wanderer.position.x;
    int newY = wanderer.position.y;

    if (!(newX + 1 >= WIDTH - 1 || newX - 1 <= 0 || newY + 1 >= LENGTH - 1 || newY - 1 <= 0)) {
        char currentTerrain = map->map[newY][newX];
        char nextTerrain = currentTerrain;

        if (wanderer.direction == 0) {
            nextTerrain = map->map[newY][newX + 1];
        } else if (wanderer.direction == 1) { 
            nextTerrain = map->map[newY][newX - 1];
        } else if (wanderer.direction == 2) { 
            nextTerrain = map->map[newY - 1][newX];
        } else if (wanderer.direction == 3) { 
            nextTerrain = map->map[newY + 1][newX];
        }

        if (nextTerrain != currentTerrain) {
            wanderer.direction = rand() % 4;
        } else {
            if (wanderer.direction == 0) { 
                newX++;
            } else if (wanderer.direction == 1) { 
                newX--;
            } else if (wanderer.direction == 2) { 
                newY--;
            } else if (wanderer.direction == 3) { 
                newY++;
            }
        }
    } else {
        wanderer.direction = rand() % 4;
    }

    wanderer.position.x = newX;
    wanderer.position.y = newY;
}

void updateExplorerLocation(Map *map) {
    int newX, newY;

    do {
        newX = explorer.position.x;
        newY = explorer.position.y;

        int randomDirection = rand() % 8;

        switch (randomDirection) {
            case 0: 
                newX++;
                break;
            case 1: 
                newX--;
                break;
            case 2: 
                newY--;
                break;
            case 3: 
                newY++;
                break;
            case 4: 
                newX++;
                newY--;
                break;
            case 5: 
                newX--;
                newY--;
                break;
            case 6: 
                newX++;
                newY++;
                break;
            case 7: 
                newX--;
                newY++;
                break;
            default:
                break;
        }

        if (newX >= 0 && newX < WIDTH && newY >= 0 && newY < LENGTH) {
            char nextTerrain = map->map[newY][newX];

            if (nextTerrain != '&' && nextTerrain != '^' && nextTerrain != '~' &&
                nextTerrain != 'h' && nextTerrain != 'r' && nextTerrain != 'p' &&
                nextTerrain != 'w' && nextTerrain != 's' && nextTerrain != '%') {
                explorer.position.x = newX;
                explorer.position.y = newY;
            }
        } else {
            explorer.direction = rand() % 8;  
        }
    } while (map->map[explorer.position.y][explorer.position.x] == '&' || 
             map->map[explorer.position.y][explorer.position.x] == '^' || 
             map->map[explorer.position.y][explorer.position.x] == '~' ||
             map->map[explorer.position.y][explorer.position.x] == 'h' || 
             map->map[explorer.position.y][explorer.position.x] == 'r' || 
             map->map[explorer.position.y][explorer.position.x] == 'p' || 
             map->map[explorer.position.y][explorer.position.x] == 'w' ||
             map->map[explorer.position.y][explorer.position.x] == 's' ||
             map->map[explorer.position.y][explorer.position.x] == '%');
}

void updatePCLocation(Map *map, int move){
    int newX, newY;

    newX = pc.position.x;
    newY = pc.position.y;

       if(move == '7' || move == 'y'){
        newX--;
        newY--;
       }else if(move == '8' || move == 'k'){
        newY--;
       }else if(move == '9' || move == 'u'){
        newX++;
        newY--;
       }else if(move =='6' || move == 'l'){
        newX++;
       }else if(move =='3' || move == 'n'){
        newX++;
        newY++;
       }else if(move =='2' || move == 'j'){
        newY++;
       }else if(move =='1' || move == 'b'){
        newX--;
        newY++;
       }else if(move =='4' || move == 'h'){
        newX--;
       }else if(move == '5' || move == '.' || move == ' '){
        newX = pc.position.x;
        newY = pc.position.y;
       }else if(move == '<' || move == 27){
        newX = pc.position.x;
        newY = pc.position.y;
       }

   if (newX >= 0 && newX < WIDTH - 1 && newY >= 0 && newY < LENGTH - 1) {
        pc.position.x = newX;
        pc.position.y = newY;
    }

}

void hikerPosFromPC(int hikerX, int hikerY, int pcX, int pcY) {
    int north = pcY - hikerY;
    int south = hikerY - pcY;
    int east = hikerX - pcX;
    int west = pcX - hikerX;

    char directionStr[100]; 

    if (north > 0) {
        sprintf(directionStr, "%d north", north);
    } else if (south > 0) {
        sprintf(directionStr, "%d south", south);
    } 

    if (west > 0) {
        sprintf(directionStr + strlen(directionStr), " by %d west", west);
    } else if (east > 0) {
        sprintf(directionStr + strlen(directionStr), " by %d east", east);
    } 

    mvprintw(4, 25, "hiker: %s", directionStr);
    refresh(); 
}

void rivalPosFromPC(int rivalX, int rivalY, int pcX, int pcY) {
    int south = rivalY - pcY;
    int north = pcY - rivalY;
    int east = rivalX - pcX;
    int west = pcX - rivalX;

    char directionStr[100]; 

    if (north > 0) {
        sprintf(directionStr, "%d north", north);
    } else if (south > 0) {
        sprintf(directionStr, "%d south", south);
    } 

    if (west > 0) {
        sprintf(directionStr + strlen(directionStr), " by %d west", west);
    } else if (east > 0) {
        sprintf(directionStr + strlen(directionStr), " by %d east", east);
    } 

    mvprintw(5, 25, "rival: %s", directionStr);
    refresh(); 
}

void pacerPosFromPC(int pacerX, int pacerY, int pcX, int pcY) {
    int south = pacerY - pcY;
    int north = pcY - pacerY;
    int east = pacerX - pcX;
    int west = pcX - pacerX;

    char directionStr[100]; 

    if (north > 0) {
        sprintf(directionStr, "%d north", north);
    } else if (south > 0) {
        sprintf(directionStr, "%d south", south);
    } 

    if (west > 0) {
        sprintf(directionStr + strlen(directionStr), " by %d west", west);
    } else if (east > 0) {
        sprintf(directionStr + strlen(directionStr), " by %d east", east);
    } 

    mvprintw(6, 25, "pacer: %s", directionStr);
    refresh(); 
}

void wandererPosFromPC(int wandererX, int wandererY, int pcX, int pcY) {
    int south = wandererY - pcY;
    int north = pcY - wandererY;
    int east = wandererX - pcX;
    int west = pcX - wandererX;

    char directionStr[100]; 

    if (north > 0) {
        sprintf(directionStr, "%d north", north);
    } else if (south > 0) {
        sprintf(directionStr, "%d south", south);
    } 

    if (west > 0) {
        sprintf(directionStr + strlen(directionStr), " by %d west", west);
    } else if (east > 0) {
        sprintf(directionStr + strlen(directionStr), " by %d east", east);
    } 

    mvprintw(7, 25, "wanderer: %s", directionStr);
    refresh(); 
}

void sentriePosFromPC(int sentrieX, int sentrieY, int pcX, int pcY) {
    int south = sentrieY - pcY;
    int north = pcY - sentrieY;
    int east = sentrieX - pcX;
    int west = pcX - sentrieX;

    char directionStr[100]; 

    if (north > 0) {
        sprintf(directionStr, "%d north", north);
    } else if (south > 0) {
        sprintf(directionStr, "%d south", south);
    } 

    if (west > 0) {
        sprintf(directionStr + strlen(directionStr), " by %d west", west);
    } else if (east > 0) {
        sprintf(directionStr + strlen(directionStr), " by %d east", east);
    } 

    mvprintw(8, 25, "sentrie: %s", directionStr);
    refresh(); 
}

void explorerPosFromPC(int explorerX, int explorerY, int pcX, int pcY) {
    int south = explorerY - pcY;
    int north = pcY - explorerY;
    int east = explorerX - pcX;
    int west = pcX - explorerX;

    char directionStr[100]; 

    if (north > 0) {
        sprintf(directionStr, "%d north", north);
    } else if (south > 0) {
        sprintf(directionStr, "%d south", south);
    } 

    if (west > 0) {
        sprintf(directionStr + strlen(directionStr), " by %d west", west);
    } else if (east > 0) {
        sprintf(directionStr + strlen(directionStr), " by %d east", east);
    } 

    mvprintw(9, 25, "explorer: %s", directionStr);
    refresh(); 
}

int isPConCorM(Map *map) {
    if (pc.position.x == pokeCenter.position.x && pc.position.y == pokeCenter.position.y) {
        return 1; 
    }

    if(pc.position.x == pokeMart.position.x && pc.position.y == pokeMart.position.y){
        return 2;
    }
    return 0; 
}

void isTrainerThere() {
 if(hasHikerBeenBattled == 0){
    isHikerThere();
 } 
  
  if(hasRivalBeenBattled == 0){
    isRivalThere();
 }
 if(hasPacerBeenBattled == 0){
    isPacerThere();;
 }
 if(hasWandererBeenBattled == 0){
    isWandererThere();
 }
 if(hasSentrieBeenBattled == 0){
    isSentrieThere();
 }
 if(hasExplorerBeenBattled == 0){
    isExplorerThere();
 }
  
}

void isHikerThere(){
    int pcX = pc.position.x;
    int pcY = pc.position.y;
    int hikerX = hiker.position.x;
    int hikerY = hiker.position.y;
    int printBattle = 0;
     if(pcX == hikerX && pcY == hikerY){
     printBattle = 1;
   }

   if(pcX-1 == hikerX && pcY == hikerY){
     printBattle = 1;
   }

   if(pcX+1 == hikerX &&  pcY == hikerY){
     printBattle = 1;
   }

   if(pcY-1 == hikerY && pcX == hikerX){
     printBattle = 1;
   }

   if(pcY+1 == hikerY && pcX == hikerX){
     printBattle = 1;
   }

    if(printBattle == 1){
        hasHikerBeenBattled = 1;
        mvprintw(22, 0, "You have entered a battle with a hiker!");
    }

}

void isRivalThere(){
    int pcX = pc.position.x;
    int pcY = pc.position.y;
    int rivalX = rival.position.x;
    int rivalY = rival.position.y;
       int printBattle = 0;
     if(pcX == rivalX && pcY == rivalY){
     printBattle = 1;
   }

   if(pcX-1 == rivalX && pcY == rivalY){
    printBattle = 1;
   }

   if(pcX+1 == rivalX &&  pcY == rivalY){
    printBattle = 1;
   }

   if(pcY-1 == rivalY && pcX == rivalX){
    printBattle = 1;
   }

   if(pcY+1 == rivalY && pcX == rivalX){
    printBattle = 1;
   }

     if(printBattle == 1){
        hasRivalBeenBattled = 1;
        mvprintw(22, 0, "You have entered a battle with a rival!");
    }

}

void isPacerThere(){
    int pcX = pc.position.x;
    int pcY = pc.position.y;
    int pacerX = pacer.position.x;
    int pacerY = pacer.position.y;
    int printBattle = 0;
     if(pcX == pacerX && pcY == pacerY){
     printBattle = 1;
   }

   if(pcX-1 == pacerX && pcY == pacerY){
    printBattle = 1;
   }

   if(pcX+1 == pacerX &&  pcY == pacerY){
    printBattle = 1;
   }

   if(pcY-1 == pacerY && pcX == pacerX){
    printBattle = 1;
   }

   if(pcY+1 == pacerY && pcX == pacerX){
    printBattle = 1;
   }

      if(printBattle == 1){
        hasPacerBeenBattled = 1;
        mvprintw(22, 0, "You have entered a battle with a pacer!");
    }

}

void isWandererThere(){
    int pcX = pc.position.x;
    int pcY = pc.position.y;
    int wandererX = wanderer.position.x;
    int wandererY = wanderer.position.y;
    int printBattle = 0;
     if(pcX == wandererX && pcY == wandererY){
     printBattle = 1;
   }

   if(pcX-1 == wandererX && pcY == wandererY){
     printBattle = 1;
   }

   if(pcX+1 == wandererX &&  pcY == wandererY){
     printBattle = 1;
   }

   if(pcY-1 == wandererY && pcX == wandererX){
     printBattle = 1;
   }

   if(pcY+1 == wandererY && pcX == wandererX){
     printBattle = 1;
   }

    if(printBattle == 1){
        hasWandererBeenBattled = 1;
        mvprintw(22, 0, "You have entered a battle with a wanderer!");
    }

}

void isSentrieThere(){
    int pcX = pc.position.x;
    int pcY = pc.position.y;
    int sentrieX = sentrie.position.x;
    int sentrieY = sentrie.position.y;
    int printBattle = 0;
     if(pcX == sentrieX && pcY == sentrieY){
     printBattle = 1;
   }

   if(pcX-1 == sentrieX && pcY == sentrieY){
    printBattle = 1;
   }

   if(pcX+1 == sentrieX &&  pcY == sentrieY){
    printBattle = 1;
   }

   if(pcY-1 == sentrieY && pcX == sentrieX){
    printBattle = 1;
   }

   if(pcY+1 == sentrieY && pcX == sentrieX){
    printBattle = 1;
   }

      if(printBattle == 1){
        hasSentrieBeenBattled = 1;
        mvprintw(22, 0, "You have entered a battle with a sentrie!");
    }

}

void isExplorerThere(){
    int pcX = pc.position.x;
    int pcY = pc.position.y;
    int explorerX = explorer.position.x;
    int explorerY = explorer.position.y;
    int printBattle = 0;
     if(pcX == explorerX && pcY == explorerY){
     printBattle = 1;
   }

   if(pcX-1 == explorerX && pcY == explorerY){
    printBattle = 1;
   }

   if(pcX+1 == explorerX &&  pcY == explorerY){
    printBattle = 1;
   }

   if(pcY-1 == explorerY && pcX == explorerX){
    printBattle = 1;
   }

   if(pcY+1 == explorerY && pcX == explorerX){
    printBattle = 1;
   }

       if(printBattle == 1){
        hasExplorerBeenBattled = 1;
        mvprintw(22, 0, "You have entered a battle with an Explorer!");
    }

}

void clearMapAroundTrainerList() {
    int startY = 3;  
    int endY = 10;   
    int startX = 22;  
    int endX = 55;    

    for (int y = startY; y <= endY; y++) {
        for (int x = startX; x <= endX; x++) {
            mvprintw(y, x, " "); 
        }
    }
}