#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h> 

#define NUM_MAPSX 401
#define NUM_MAPSY 401
#define LENGTH 21
#define WIDTH 80

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

typedef struct {
    char characterChar;
    int movementCost[11];
} Trainer;

//using INT_MAX for infinity costs
Trainer trainerTypes[] = {
    {'h', {INT_MAX, INT_MAX, 10, 50, 50, 15, 10, 15, 15, INT_MAX, INT_MAX}}, // hiker
    {'r', {INT_MAX, INT_MAX, 10, 50, 50, 20, 10, INT_MAX, INT_MAX, INT_MAX, INT_MAX}}  // rival
};

// Define a structure to represent a map
typedef struct {
    char map[LENGTH][WIDTH];
} Map;

int currentDirection;

/*-----FUNCTION PROTOTYPES---------*/
void generateMap(Map *map);
void printMap(Map *map);
void fillMapGrass(Map *map);
void generateBorder(Map *map);
void genPathCM(Map *map, char direction);
void generateTerrain(Map *map);
int getRandom(int min, int max);
int isPathThere(char symbol, int x, int y, int width, int height, Map *map);
void calculateDistanceMap(Map *map, int distanceMap[][WIDTH], Trainer trainerTypes[], TerrainType terrainTypes[], int numTerrainTypes);
void printDistanceMap(int distanceMap[][WIDTH]);

int main(int argc, char* argv[]) {
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
    if (pathGenerated[currentMapX][currentMapY] == 0) {
        genPathCM(&maps[currentMapX][currentMapY], currentDirection);
        pathGenerated[currentMapX][currentMapY] = 1;
    }

    int distanceMap[LENGTH][WIDTH] = {0};

    while (1) {
        printMap(&maps[currentMapX][currentMapY]);
        printf("Current Map Coordinates: (%d, %d):\n", currentMapX, currentMapY);
        fflush(stdout);

        calculateDistanceMap(&maps[currentMapX][currentMapY], distanceMap, trainerTypes, terrainTypes, sizeof(terrainTypes) / sizeof(terrainTypes[0]));

        printf("Distance Map:\n");
        printDistanceMap(distanceMap);

        printf("Enter 'n' to move north, 's' to move south, 'w' to move west, 'e' to move east, or 'q' to quit: ");
        char input;
        scanf(" %c", &input);

        if (input == 'n' && currentMapY > 0) {
            currentMapY--;
            currentDirection = 'n';
            if (pathGenerated[currentMapX][currentMapY] == 0) {
                genPathCM(&maps[currentMapX][currentMapY], currentDirection);
                pathGenerated[currentMapX][currentMapY] = 1;
            }
        } else if (input == 's' && currentMapY < NUM_MAPSY - 1) {
            currentMapY++;
            currentDirection = 's';
            if (pathGenerated[currentMapX][currentMapY] == 0) {
                genPathCM(&maps[currentMapX][currentMapY], currentDirection);
                pathGenerated[currentMapX][currentMapY] = 1;
            }
        } else if (input == 'w' && currentMapX > 0) {
            currentMapX--;
            currentDirection = 'w';
            if (pathGenerated[currentMapX][currentMapY] == 0) {
                genPathCM(&maps[currentMapX][currentMapY], currentDirection);
                pathGenerated[currentMapX][currentMapY] = 1;
            }
        } else if (input == 'e' && currentMapX < NUM_MAPSX - 1) {
            currentMapX++;
            currentDirection = 'e';
            if (pathGenerated[currentMapX][currentMapY] == 0) {
                genPathCM(&maps[currentMapX][currentMapY], currentDirection);
                pathGenerated[currentMapX][currentMapY] = 1;
            }
        } else if(input == 'f'){
            int inputX;
            int inputY;
            printf("enter an x and y coordinate separated by a comma: ");
            scanf("%d,%d", &inputX, &inputY);
            if (inputX >= 0 && inputX < NUM_MAPSX && inputY >= 0 && inputY < NUM_MAPSY) {
                currentMapX = inputX;
                currentMapY = inputY;
                if (pathGenerated[currentMapX][currentMapY] == 0) {
                    genPathCM(&maps[currentMapX][currentMapY], currentDirection);
                    pathGenerated[currentMapX][currentMapY] = 1;
                }
            } else {
                printf("Invalid coordinates. Try again.\n");
            }
        } else if (input == 'q') {
            break;
        } else {
            printf("Invalid input. Try again.\n");
        }
        printf("current direction: %c\n", currentDirection);
    }

    for (int x = 0; x < NUM_MAPSX; x++) {
        free(maps[x]);
    }
    free(maps);
    return 0;
}


void generateMap(Map *map) {
    fillMapGrass(map);
    generateBorder(map);
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

    int pcX, pcY;
    do {
        pcX = rand() % (WIDTH - 3) + 1;  // Random x position within the path
        pcY = rand() % (LENGTH - 4) + 2; // Random y position within the path
    } while (map->map[pcY][pcX] != '#'); // Keep trying until a path location is found

    map->map[pcY][pcX] = '@'; 
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

void calculateDistanceMap(Map *map, int distanceMap[][WIDTH], Trainer trainerTypes[], TerrainType terrainTypes[], int numTerrainTypes) {
    int largeValue = 10000;
    for (int y = 0; y < LENGTH; y++) {
        for (int x = 0; x < WIDTH; x++) {
            distanceMap[y][x] = largeValue;
        }
    }

    for (int y = 0; y < LENGTH; y++) {
        for (int x = 0; x < WIDTH; x++) {
            char terrainChar = map->map[y][x];
            int trainerIndex = -1;

            for (int i = 0; i < numTerrainTypes; i++) {
                if (trainerTypes[i].characterChar == terrainChar) {
                    trainerIndex = i;
                    break;
                }
            }

            if (trainerIndex >= 0) {
                int terrainIndex = 2;
                distanceMap[y][x] = trainerTypes[trainerIndex].movementCost[terrainIndex];
            } else {
                for (int i = 0; i < numTerrainTypes; i++) {
                    if (terrainTypes[i].terrainChar == terrainChar) {
                        int terrainIndex = i;
                        distanceMap[y][x] = trainerTypes[0].movementCost[terrainIndex];
                        break;
                    }
                }
            }
        }
    }
}

void printDistanceMap(int distanceMap[][WIDTH]) {
    for (int y = 0; y < LENGTH; y++) {
        for (int x = 0; x < WIDTH; x++) {

            printf("%d ", distanceMap[y][x]);
        }
        printf("\n");
    }
}