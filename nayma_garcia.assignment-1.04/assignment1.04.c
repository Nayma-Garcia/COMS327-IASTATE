#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <limits.h> 
#include <string.h>

#define LENGTH 21
#define WIDTH 80

typedef struct {
    char map[LENGTH][WIDTH];
} Map;

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
    {':', 4, 10, 4, 13, 4, 9},  // tall grass
    {'^', 2, 6, 4, 13, 4, 9},   // trees
    {'~', 1, 5, 4, 13, 4, 9},   // water
    {'%', 1, 5, 4, 13, 4, 9},   // boulders
};

typedef struct {
    char characterChar;
    int movementCost[11];
} Trainer;

Trainer trainerTypes[] = {
    {'h', {INT_MAX, INT_MAX, 10, 10, 10, 2, 10, INT_MAX, INT_MAX, INT_MAX, 10}}, // hiker
    {'r', {INT_MAX, INT_MAX, 10, 50, 50, 15, 10, 15, 15, INT_MAX, INT_MAX}},  // rival
    {'s', {INT_MAX, INT_MAX, 10, 50, 50, 20, 10, INT_MAX, INT_MAX, INT_MAX, INT_MAX}} , //sentire
    {'w', {INT_MAX, INT_MAX, 10, 50, 50, 20, 10, INT_MAX, INT_MAX, INT_MAX, INT_MAX}} , //wanderer
    {'p', {INT_MAX, INT_MAX, 10, 50, 50, 20, 10, INT_MAX, INT_MAX, INT_MAX, INT_MAX}} , //pacer
    {'e', {INT_MAX, INT_MAX, 10, 50, 50, 20, 10, INT_MAX, INT_MAX, INT_MAX, INT_MAX}}  // explorer
};

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

int hikerDirection = 0; 
int rivalDirection = 0; 

void generateMap(Map *map, int numtrainer);
void printMap(Map *map);
void fillMapGrass(Map *map);
void generateBorder(Map *map);
void genPathCM(Map *map);
void generateTerrain(Map *map);
int getRandom(int min, int max);
int isPathThere(char symbol, int x, int y, int width, int height, Map *map);
void updateHikerLocation(Map *map);
void updateRivalLocation(Map *map);
void updatePacerLocation(Map *map);
void updateWandererLocation(Map *map);
void updateExplorerLocation(Map *map);

int numtrainers = 10;

int main(int argc, char* argv[]) {
    if (argc > 2) {
        printf("Usage: %s [<numtrainers>]\n", argv[0]);
        return 1;
    }

    if (argc == 2) {
        numtrainers = atoi(argv[1]);
        if (numtrainers < 0) {
            printf("Number of trainers must be non-negative.\n");
            return 1;
        }
    } else {
        numtrainers = 10;
    }

    printf("%d\n", numtrainers);

    srand(time(NULL));

    Map map;
    generateMap(&map, numtrainers);

    while (1) {
        printMap(&map);
        updateHikerLocation(&map);
        updateRivalLocation(&map);
        updatePacerLocation(&map);
        updateWandererLocation(&map);
        updateExplorerLocation(&map);
        fflush(stdout);
        usleep(500000);
    }

    return 0;
}




void generateMap(Map *map, int numtrainers) {
    fillMapGrass(map);
    generateBorder(map);
    genPathCM(map);
    generateTerrain(map);

        pc.symbol = '@';
    do {
        pc.position.x = getRandom(1, WIDTH - 2);
        pc.position.y = getRandom(1, LENGTH - 2);
    } while (map->map[pc.position.y][pc.position.x] != '#');

    for (int i = 0; i <= numtrainers; i++){
        hiker.symbol = 'h';
    do {
        hiker.position.x = getRandom(1, WIDTH - 2);
        hiker.position.y = getRandom(1, LENGTH - 2);
    } while (map->map[hiker.position.y][hiker.position.x] == '#');

    rival.symbol = 'r';
    do {
        rival.position.x = getRandom(1, WIDTH - 2);
        rival.position.y = getRandom(1, LENGTH - 2);
    } while (map->map[rival.position.y][rival.position.x] == '#' ||
             (rival.position.x == hiker.position.x && rival.position.y == hiker.position.y) ||
             (rival.position.x == pc.position.x && rival.position.y == pc.position.y));

    pacer.symbol = 'p';
    do {
        pacer.position.x = getRandom(1, WIDTH - 2);
        pacer.position.y = getRandom(1, LENGTH - 2);
    } while (map->map[pacer.position.y][pacer.position.x] == '#' ||
             (pacer.position.x == hiker.position.x && pacer.position.y == hiker.position.y) ||
             (pacer.position.x == pc.position.x && pacer.position.y == pc.position.y) ||
             (pacer.position.x == rival.position.x && pacer.position.y == rival.position.y));

    pacer.direction = 0; 

   wanderer.symbol = 'w';
    do {
        wanderer.position.x = getRandom(1, WIDTH - 2);
        wanderer.position.y = getRandom(1, LENGTH - 2);
    } while ((map->map[wanderer.position.y][wanderer.position.x] != ':') || 
             (wanderer.position.x == hiker.position.x && wanderer.position.y == hiker.position.y) ||
             (wanderer.position.x == pc.position.x && wanderer.position.y == pc.position.y) ||
             (wanderer.position.x == rival.position.x && wanderer.position.y == rival.position.y) ||
             (wanderer.position.x == pacer.position.x && wanderer.position.y == pacer.position.y));

    wanderer.direction = getRandom(0, 3);

 sentrie.symbol = 's'; 

do {
    sentrie.position.x = getRandom(1, WIDTH - 2);
    sentrie.position.y = getRandom(1, LENGTH - 2);
} while (map->map[sentrie.position.y][sentrie.position.x] != '.');
map->map[sentrie.position.y][sentrie.position.x] = sentrie.symbol;

explorer.symbol = 'e';
do {
    explorer.position.x = getRandom(1, WIDTH - 2);
    explorer.position.y = getRandom(1, LENGTH - 2);
} while ((map->map[explorer.position.y][explorer.position.x] != ':') || 
         (explorer.position.x == hiker.position.x && explorer.position.y == hiker.position.y) ||
         (explorer.position.x == pc.position.x && explorer.position.y == pc.position.y) ||
         (explorer.position.x == rival.position.x && explorer.position.y == rival.position.y) ||
         (explorer.position.x == pacer.position.x && explorer.position.y == pacer.position.y) ||
         (explorer.position.x == wanderer.position.x && explorer.position.y == wanderer.position.y) ||
         (explorer.position.x == sentrie.position.x && explorer.position.y == sentrie.position.y));

explorer.direction = getRandom(0, 3);
    }
    
}




void printMap(Map *map) {
    int x;
    int y;
    for (y = 0; y < LENGTH; y++) {
        for (x = 0; x < WIDTH; x++) {
            if (x == sentrie.position.x && y == sentrie.position.y) {
                printf("%c", sentrie.symbol);
            } else if (x == hiker.position.x && y == hiker.position.y) {
                printf("%c", hiker.symbol);
            } else if (x == pc.position.x && y == pc.position.y) {
                printf("%c", pc.symbol);
            } else if (x == rival.position.x && y == rival.position.y) {
                printf("%c", rival.symbol);
            } else if (x == pacer.position.x && y == pacer.position.y) {
                printf("%c", pacer.symbol);
            } else if (x == wanderer.position.x && y == wanderer.position.y) {
                printf("%c", wanderer.symbol);
            } else if (x == explorer.position.x && y == explorer.position.y) {  
                printf("%c", explorer.symbol);
            } else {
                printf("%c", map->map[y][x]);
            }
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
        map->map[0][x] = '%'; 
        map->map[LENGTH - 1][x] = '%';
    }
    for (y = 0; y < LENGTH; y++) {
        map->map[y][0] = '%';
        map->map[y][WIDTH - 1] = '%';
    }
}

void genPathCM(Map *map) {
    int x, y;

    // Making the path that goes from east to west
    y = rand() % (LENGTH - 3) + 1;  // So it doesn't touch the top border
    for (x = 0; x < WIDTH; x++) {
        map->map[y][x] = '#';
    }

    x = rand() % (WIDTH - 3) + 1;  
    if (y < LENGTH - 1 && x < WIDTH - 1) {
        map->map[y + 1][x] = 'C';
    } else {
        map->map[y - 1][x] = 'C';
    }

    if (y < LENGTH - 1 && x < WIDTH - 1) {
        map->map[y + 1][x + 1] = 'M';
    } else if (map->map[y][x] == '#') {
        map->map[y][x + 1] = 'M';
    } else {
        map->map[y - 1][x - 1] = 'M';
    }

    x = rand() % (WIDTH - 3); 
    for (y = 0; y < LENGTH; y++) {
        map->map[y][x] = '#';
    }
}





void generateTerrain(Map *map) {
    int numTerrainTypes = sizeof(terrainTypes) / sizeof(terrainTypes[0]);

    for (int i = 0; i < numTerrainTypes; i++) {
        TerrainType terrainType = terrainTypes[i];
        int x, y;

        do {
            x = getRandom(1, WIDTH - terrainType.maxWidth - 1);
            y = getRandom(1, LENGTH - terrainType.maxHeight - 1);
        } while (isPathThere('#', x, y, terrainType.maxWidth, terrainType.maxHeight, map));

        for (int ty = y; ty < y + terrainType.maxHeight; ty++) {
            for (int tx = x; tx < x + terrainType.maxWidth; tx++) {
                map->map[ty][tx] = terrainType.terrainChar;
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
