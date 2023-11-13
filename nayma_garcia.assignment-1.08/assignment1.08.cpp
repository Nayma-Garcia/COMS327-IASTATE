#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <limits.h> 
#include <string.h>
#include <ncurses.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <random>
#include <cmath>

#define MAPSX 401
#define MAPSY 401
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

class Map {
public:
    char map[LENGTH][WIDTH];
};

class TerrainType {
public:
    char terrainChar;
    int minRegions;
    int maxRegions;
    int minWidth;
    int maxWidth;
    int minHeight;
    int maxHeight;
};

TerrainType terrainTypes[] = {
    {':', 4, 10, 4, 13, 4, 9},  // tall grass
    {'^', 2, 6, 4, 13, 4, 9},   // trees
    {'~', 1, 5, 4, 13, 4, 9},   // water
    {'%', 1, 5, 4, 13, 4, 9},   // boulders
};

class Trainer {
public:
    char characterChar;
    int movementCost[11];
};

Trainer trainerTypes[] = {
    {'h', {INT_MAX, INT_MAX, 10, 10, 10, 2, 10, INT_MAX, INT_MAX, INT_MAX, 10}}, // hiker
    {'r', {INT_MAX, INT_MAX, 10, 50, 50, 15, 10, 15, 15, INT_MAX, INT_MAX}},  // rival
    {'s', {INT_MAX, INT_MAX, 10, 50, 50, 20, 10, INT_MAX, INT_MAX, INT_MAX, INT_MAX}} , // sentire
    {'w', {INT_MAX, INT_MAX, 10, 50, 50, 20, 10, INT_MAX, INT_MAX, INT_MAX, INT_MAX}} , // wanderer
    {'p', {INT_MAX, INT_MAX, 10, 50, 50, 20, 10, INT_MAX, INT_MAX, INT_MAX, INT_MAX}} , // pacer
    {'e', {INT_MAX, INT_MAX, 10, 50, 50, 20, 10, INT_MAX, INT_MAX, INT_MAX, INT_MAX}}  // explorer
};


class Coordinate {
public:
    int x;
    int y;
};

class Hiker {
public:
    char symbol;
    Coordinate position;
};

class PC {
public:
    char symbol;
    Coordinate position;
};

class Rival {
public:
    char symbol;
    Coordinate position;
};

class Pacer {
public:
    char symbol;
    Coordinate position;
    int direction;
};

class Wanderer {
public:
    char symbol;
    Coordinate position;
    int direction;
};

class Sentrie {
public:
    char symbol;
    Coordinate position;
};

class Explorer {
public:
    char symbol;
    Coordinate position;
    int direction;
};

class PokeCenter {
public:
    char symbol;
    Coordinate position;
    int direction;
};

class PokeMart {
public:
    char symbol;
    Coordinate position;
    int direction;
};

class PathWE {
public:
    char symbol;
    Coordinate position;
    int direction;
};

class PathSN {
public:
    char symbol;
    Coordinate position;
    int direction;
};

// Declare global instances
Hiker hiker;
PC pc;
Rival rival;
Pacer pacer;
Wanderer wanderer;
Sentrie sentrie;
Explorer explorer;
PokeCenter pokeCenter;
PokeMart pokeMart;
PathWE ewPath;
PathSN snPath;

int hikerDirection = 0; 
int rivalDirection = 0; 

int hasHikerBeenBattled = 0;
int hasRivalBeenBattled = 0;
int hasPacerBeenBattled = 0;
int hasWandererBeenBattled = 0;
int hasSentrieBeenBattled = 0;
int hasExplorerBeenBattled = 0;

void generateMap(Map *map);
void printMap(Map *map);
void fillMapGrass(Map *map);
void generateBorder(Map *map);
void genPathCM(Map *map, int nsX, int ewY);
void generateTerrain(Map *map);
int getRandom(int min, int max);
int isPathThere(char symbol, int x, int y, int width, int height, Map *map);
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
void setNewXforPC(int x);
void setNewYforPC(int y);
int getNewXforPC();
int getNewYforPC();
void placePC(int pcX, int pcY);
void placePCFly(Map *map);
void printCSV();
void parseAndPrintStatsFile();
void parseAndPrintPokemonFile();
void parseAndPrintMovesFile();
void parseAndPrintPokemonMovesFile();
void parseAndPrintPokemonSpeciesFile();
void parseAndPrintExperienceFile();
void parseAndPrintPokemonStatsFile();
void parseAndPrintTypeNamesFile();



int numtrainers = 0;
    int currentMapX = 200;
    int currentMapY = 200;

    int pathGenerated[MAPSX][MAPSY] = {0};

int newXforPC;
int newYforPC;


void generateMap(Map *map) {
    fillMapGrass(map);
    generateBorder(map);
    //genPathCM(map);
    generateTerrain(map);

        hiker.symbol = 'h';
        hiker.position.x = getRandom(1, WIDTH - 2);
        hiker.position.y = getRandom(1, LENGTH - 2);


    rival.symbol = 'r';
        rival.position.x = getRandom(1, WIDTH - 2);
        rival.position.y = getRandom(1, LENGTH - 2);

    pacer.symbol = 'p';

        pacer.position.x = getRandom(1, WIDTH - 2);
        pacer.position.y = getRandom(1, LENGTH - 2);

    pacer.direction = 0; 

   wanderer.symbol = 'w';
        wanderer.position.x = getRandom(1, WIDTH - 2);
        wanderer.position.y = getRandom(1, LENGTH - 2);
    wanderer.direction = getRandom(0, 3);

 sentrie.symbol = 's'; 


    sentrie.position.x = getRandom(1, WIDTH - 2);
    sentrie.position.y = getRandom(1, LENGTH - 2);
map->map[sentrie.position.y][sentrie.position.x] = sentrie.symbol;

explorer.symbol = 'e';
    explorer.position.x = getRandom(1, WIDTH - 2);
    explorer.position.y = getRandom(1, LENGTH - 2);

explorer.direction = getRandom(0, 3);
    
}

void placePC(int pcX, int pcY){
        pc.position.x = pcX;
        pc.position.y = pcY;
}

void placePCFly(Map *map){
    pc.symbol = '@';
      do {
        pc.position.x = getRandom(1, WIDTH - 2);
        pc.position.y = getRandom(1, LENGTH - 2);
    } while (map->map[pc.position.y][pc.position.x] != '#');
}


//WHAT TO DO NEXT : IDEAS

//make new function to place pc at certain x or y based on where it ended, for example, if pc was at width in last map, place it at the same y but 1 for the x in the new map (1 so it doesnt trigger moving to a diff map)

//for path, pass in current path x and y, and place path in new map accordingly, if i pass in a 0 for the x or y, then that means to generate a new random value (this is for when moving along the x axis lets say, only the e-w needs to stay the same, so then i would pass in 0 for the n-s path to generate a new place for that)


void printMap(Map *map) {
    int x;
    int y;
    clear();
    for (y = 0; y < LENGTH; y++) {
        for (x = 0; x < WIDTH; x++) {
            if (x == sentrie.position.x && y == sentrie.position.y) {
                mvaddch(y, x, sentrie.symbol);
            } else if (x == hiker.position.x && y == hiker.position.y) {
                mvaddch(y,x, hiker.symbol);
            } else if (x == pc.position.x && y == pc.position.y) {
                attron(COLOR_PAIR(PLAYER_PAIR));
                 mvaddch(y,x, pc.symbol);
                attroff(COLOR_PAIR(PLAYER_PAIR));
            } else if (x == rival.position.x && y == rival.position.y) {
                 mvaddch(y,x, rival.symbol);
            } else if (x == pacer.position.x && y == pacer.position.y) {
                 mvaddch(y,x, pacer.symbol);
            } else if (x == wanderer.position.x && y == wanderer.position.y) {
                 mvaddch(y,x, wanderer.symbol);
            } else if (x == explorer.position.x && y == explorer.position.y) {  
                 mvaddch(y,x, explorer.symbol);
            } else {
                 mvaddch(y,x, map->map[y][x]);
            }
        }
        printf("\n");
    }
    refresh();
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
    for (x = 0; x <= WIDTH; x++) {
        map->map[0][x] = '%'; 
        map->map[LENGTH - 1][x] = '%';
    }
    for (y = 0; y <= LENGTH + 1; y++) {
        map->map[y][0] = '%';
        map->map[y][WIDTH - 1] = '%';
    }
}

void genPathCM(Map *map, int nsX, int ewY) {
    int x, y;
    // Making the path that goes from east to west
    if(ewY == 0){
        y = rand() % (LENGTH - 3) + 1;  // So it doesn't touch the top border
    }else{
        y = ewY;
    }
    for (x = 0; x < WIDTH; x++) {
        ewPath.symbol = '#';
        ewPath.position.y = y;
        map->map[y][x] = ewPath.symbol;
    }

    x = rand() % (WIDTH - 3) + 1;  
    if (y < LENGTH - 1 && x < WIDTH - 1) {
        pokeCenter.symbol = 'C';
        pokeCenter.position.x = x;
        pokeCenter.position.y = y + 1;
    } else {
        pokeCenter.symbol = 'C';
        pokeCenter.position.x = x;
        pokeCenter.position.y = y - 1;
    }

    if (y < LENGTH - 1 && x < WIDTH - 1) {
        pokeMart.symbol = 'M';
        pokeMart.position.x = x + 1;
        pokeMart.position.y = y + 1;
    } else if (map->map[y][x] == '#') {
        pokeMart.symbol = 'M';
        pokeMart.position.x = x + 1;
        pokeMart.position.y = y;
    } else {
        pokeMart.symbol = 'M';
        pokeMart.position.x = x - 1;
        pokeMart.position.y = y - 1;
    }

    map->map[pokeCenter.position.y][pokeCenter.position.x] = pokeCenter.symbol;
    map->map[pokeMart.position.y][pokeMart.position.x] = pokeMart.symbol;

    if(nsX == 0){
         x = rand() % (WIDTH - 3); 
    }else{
        x = nsX;
    }
  
   
    for (y = 0; y < LENGTH; y++) {
        snPath.symbol = '#';
        snPath.position.x = x;
        map->map[y][x] = snPath.symbol;
    }

}


void generateTerrain(Map *map) {
    int numTerrainTypes = sizeof(terrainTypes) / sizeof(terrainTypes[0]);

    for (int i = 0; i < numTerrainTypes; i++) {
        TerrainType terrainType = terrainTypes[i];

        if (terrainType.terrainChar == ':') {  // Check if tall grass
            for (int j = 0; j < 5; j++) {  // Adjust the number of tall grass regions
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
    }

    // Then, generate other terrains
    for (int i = 0; i < numTerrainTypes; i++) {
        TerrainType terrainType = terrainTypes[i];

        if (terrainType.terrainChar != ':') {  // Skip tall grass
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

    refresh();
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

       setNewXforPC(newX);
       setNewYforPC(newY);

        if(map->map[newY][newX] != '%' && map->map[newY][newX] != '^' && map->map[newY][newX] != '~' ){
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


class Stat {
public:
    int id;
    int damage_class_id;
    std::string identifier;
    int is_battle_only;
    int game_index;
};

  std::vector<Stat> stats;

void parseAndPrintStatsFile() {
    const char *dbpath = "/share/cs327/pokedex/pokedex/data/csv/";
    const char *filename = "stats.csv";

    std::string filepath = std::string(dbpath) + filename;

    std::ifstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "Error opening the file" << std::endl;
        return; // Return early if the file can't be opened.
    }


    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        Stat stat;

        // Parse the CSV line into the Stat class members
        if (std::getline(iss, token, ','))
            stat.id = std::atoi(token.c_str()); // Use atoi to handle conversion
        if (std::getline(iss, token, ','))
            stat.damage_class_id = token.empty() ? 0 : std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            stat.identifier = token;
        if (std::getline(iss, token, ','))
            stat.is_battle_only = token == "1";
        if (std::getline(iss, token))
            stat.game_index = token.empty() ? 0 : std::atoi(token.c_str());

        // Print the CSV line as it is
        // std::cout << line << std::endl;
             stats.push_back(stat);
    }

    file.close();
}

void parseAndPrintStatsHomeFile() {
    const char *filename = "stats.csv";

    // Get the home directory
    const char *homeDir = getenv("HOME");
    if (homeDir == nullptr) {
        std::cerr << "Error: Unable to get the home directory" << std::endl;
        return;
    }

    // Construct the full file path
    std::string filepath = std::string(homeDir) + "/.poke327/pokedex/pokedex/data/csv/" + filename;

    std::ifstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "Error: Database not found" << std::endl;
        return; // Return early if the file can't be opened.
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        Stat stat;

        // Parse the CSV line into the Stat class members
        if (std::getline(iss, token, ','))
            stat.id = std::atoi(token.c_str()); // Use atoi to handle conversion
        if (std::getline(iss, token, ','))
            stat.damage_class_id = token.empty() ? 0 : std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            stat.identifier = token;
        if (std::getline(iss, token, ','))
            stat.is_battle_only = token == "1";
        if (std::getline(iss, token))
            stat.game_index = token.empty() ? 0 : std::atoi(token.c_str());

        // Print the CSV line as it is
        // std::cout << line << std::endl;
         stats.push_back(stat);
    }

    file.close();
}


class Pokemon {
public:
    int id;
    std::string identifier;
    int species_id;
    int height;
    int weight;
    int base_experience;
    int order;
    int is_default;

    int iv_hp;
    int iv_attack;
    bool is_shiny;
    int levelGet;

    // Initialization function

    int calculateHP(){
         int iv = rand() % 8192 == 0;
        int HP = std::floor(((base_experience + iv) * 2 * levelGet) / 100) + 1 + 10;
        return HP;
    }
      int calculateAttack(){
         int iv = rand() % 8192 == 0;
        int attack = std::floor(((base_experience + iv) * 2 * levelGet) / 100) + 5;
        return attack;
    }

    void initializeAttributes() {
        // Generate IVs for HP and Attack in the range [0, 15]
        iv_hp = calculateHP();
        iv_attack = calculateAttack();

        // Determine shininess
        int shinyprob = rand() % 100;
        if(shinyprob < 2){
            is_shiny = true;
        }else{
            is_shiny = false;
        }
    }
};

 std::vector<Pokemon> pokemonV;

void parseAndPrintPokemonFile() {
    const char *dbpath = "/share/cs327/pokedex/pokedex/data/csv/";
    const char *filename = "pokemon.csv";

    std::string filepath = std::string(dbpath) + filename;

    std::ifstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "Error opening the file" << std::endl;
        return; // Return early if the file can't be opened.
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        Pokemon pokemon;

        if (std::getline(iss, token, ','))
            pokemon.id = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            pokemon.identifier = token;
        if (std::getline(iss, token, ','))
            pokemon.species_id = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            pokemon.height = std::atoi(token.c_str()); // Assuming this field should be an integer
        if (std::getline(iss, token, ','))
            pokemon.weight = std::atoi(token.c_str()); // Assuming this field should be an integer
        if (std::getline(iss, token, ','))
            pokemon.base_experience = std::atoi(token.c_str()); // Assuming this field should be an integer
        if (std::getline(iss, token, ','))
            pokemon.order = std::atoi(token.c_str()); // Assuming this field should be an integer
        if (std::getline(iss, token))
            pokemon.is_default = (token == "1"); // Assuming this field should be a boolean

        // Print the CSV line as it is
        // std::cout << line << std::endl;
        pokemonV.push_back(pokemon);

    }


    file.close();
}


void parseAndPrintPokemonHomeFile() {
   const char *filename = "pokemon.csv";

    // Get the home directory
    const char *homeDir = getenv("HOME");
    if (homeDir == nullptr) {
        std::cerr << "Error: Unable to get the home directory" << std::endl;
        return;
    }

    // Construct the full file path
    std::string filepath = std::string(homeDir) + "/.poke327/pokedex/pokedex/data/csv/" + filename;

    std::ifstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "Error: Database not found" << std::endl;
        return; // Return early if the file can't be opened.
    }

    // std::string header;
    // if (std::getline(file, header)) {
    //     // Print the header line
    //     std::cout << header << std::endl;
    // }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        Pokemon pokemon;

        if (std::getline(iss, token, ','))
            pokemon.id = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            pokemon.identifier = token;
        if (std::getline(iss, token, ','))
            pokemon.species_id = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            pokemon.height = std::atoi(token.c_str()); // Assuming this field should be an integer
        if (std::getline(iss, token, ','))
            pokemon.weight = std::atoi(token.c_str()); // Assuming this field should be an integer
        if (std::getline(iss, token, ','))
            pokemon.base_experience = std::atoi(token.c_str()); // Assuming this field should be an integer
        if (std::getline(iss, token, ','))
            pokemon.order = std::atoi(token.c_str()); // Assuming this field should be an integer
        if (std::getline(iss, token))
            pokemon.is_default = (token == "1"); // Assuming this field should be a boolean

        // Print the CSV line as it is
        // std::cout << line << std::endl;
        pokemonV.push_back(pokemon);

    }


    file.close();
}
//id,   identifier,   generation_id,    type_id,     power,     pp,     accuracy,     priority,      target_id,     damage_class_id,    effect_id,     effect_chance,      contest_type_id,    contest_effect_id,        super_contest_effect_id


class Moves {
public:
    int id;
    std::string identifier;
    int generation_id;
    int type_id;
    int power;
    int pp;
    int accuracy;
    int priority;
    int target_id;
    int damage_class_id;
    int effect_id;
    int effect_chance;
    int contest_type_id;
    int contest_effect_id;
    int super_contest_effect_id;
};

 std::vector<Moves> movesV;


void parseAndPrintMovesFile() {
    const char *dbpath = "/share/cs327/pokedex/pokedex/data/csv/";
    const char *filename = "moves.csv";

    std::string filepath = std::string(dbpath) + filename;

    std::ifstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "Error opening the file" << std::endl;
        return; // Return early if the file can't be opened.
    }

    // std::string header;
    // if (std::getline(file, header)) {
    //     std::cout << header << std::endl;
    // }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        Moves move; 

        if (std::getline(iss, token, ','))
            move.id = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            move.identifier = token;
        if (std::getline(iss, token, ','))
            move.generation_id = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            move.type_id = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            move.power = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            move.pp = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            move.accuracy = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            move.priority = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            move.target_id = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            move.damage_class_id = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            move.effect_id = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            move.effect_chance = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            move.contest_type_id = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            move.contest_effect_id = std::atoi(token.c_str());
        if (std::getline(iss, token))
            move.super_contest_effect_id = std::atoi(token.c_str());

        // std::cout << line << std::endl;
         movesV.push_back(move);

    }

    file.close();
}

void parseAndPrintMovesHomeFile() {
    const char *filename = "moves.csv";

    // Get the home directory
    const char *homeDir = getenv("HOME");
    if (homeDir == nullptr) {
        std::cerr << "Error: Unable to get the home directory" << std::endl;
        return;
    }

    // Construct the full file path
    std::string filepath = std::string(homeDir) + "/.poke327/pokedex/pokedex/data/csv/" + filename;

    std::ifstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "Error: Database not found" << std::endl;
        return; // Return early if the file can't be opened.
    }

    // std::string header;
    // if (std::getline(file, header)) {
    //     std::cout << header << std::endl;
    // }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        Moves move; 

        if (std::getline(iss, token, ','))
            move.id = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            move.identifier = token;
        if (std::getline(iss, token, ','))
            move.generation_id = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            move.type_id = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            move.power = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            move.pp = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            move.accuracy = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            move.priority = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            move.target_id = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            move.damage_class_id = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            move.effect_id = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            move.effect_chance = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            move.contest_type_id = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            move.contest_effect_id = std::atoi(token.c_str());
        if (std::getline(iss, token))
            move.super_contest_effect_id = std::atoi(token.c_str());

        // std::cout << line << std::endl;
         movesV.push_back(move);
         

    }

    file.close();
}


class PokemonMoves {
public:
    int pokemon_id; 
    int version_group_id;
    int move_id;
    int pokemon_move_method_id;
    int level;
    int order;

 static std::vector<PokemonMoves> pokeMovesV;

//WHY IS THIS NOT WORKING
    void parseAndPrintPokemonMovesFile() {
        const char *dbpath = "/share/cs327/pokedex/pokedex/data/csv/";
        const char *filename = "pokemon_moves.csv";

        std::string filepath = std::string(dbpath) + filename;

        std::ifstream file(filepath);

        if (!file.is_open()) {
            std::cerr << "Error opening the file" << std::endl;
            return; 
        }

        // std::string header;
        // if (std::getline(file, header)) {
        //     std::cout << header << std::endl;
        // }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string token;
            PokemonMoves pokeMoves;

            if (std::getline(iss, token, ','))
                pokeMoves.pokemon_id = std::atoi(token.c_str());
            if (std::getline(iss, token, ','))
                pokeMoves.version_group_id = std::atoi(token.c_str());
            if (std::getline(iss, token, ','))
                pokeMoves.move_id = std::atoi(token.c_str());
            if (std::getline(iss, token, ','))
                pokeMoves.pokemon_move_method_id = std::atoi(token.c_str());
            if (std::getline(iss, token, ','))
                pokeMoves.level = std::atoi(token.c_str());
            if (std::getline(iss, token, ','))
                pokeMoves.order = std::atoi(token.c_str());
            

            // std::cout << line << std::endl;
            pokeMovesV.push_back(pokeMoves);

        }

        file.close();
    }

     void parseAndPrintPokemonMovesHomeFile() {
        const char *filename = "pokemon_moves.csv";

        // Get the home directory
        const char *homeDir = getenv("HOME");
        if (homeDir == nullptr) {
            std::cerr << "Error: Database not found" << std::endl;
            return;
        }

        // Construct the full file path
        std::string filepath = std::string(homeDir) + "/.poke327/pokedex/pokedex/data/csv/" + filename;

        std::ifstream file(filepath);

        if (!file.is_open()) {
            std::cerr << "Error opening the file" << std::endl;
            return; // Return early if the file can't be opened.
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string token;
             PokemonMoves pokeMoves;

            if (std::getline(iss, token, ','))
                pokeMoves.pokemon_id = std::atoi(token.c_str());
            if (std::getline(iss, token, ','))
                pokeMoves.version_group_id = std::atoi(token.c_str());
            if (std::getline(iss, token, ','))
                pokeMoves.move_id = std::atoi(token.c_str());
            if (std::getline(iss, token, ','))
                pokeMoves.pokemon_move_method_id = std::atoi(token.c_str());
            if (std::getline(iss, token, ','))
                pokeMoves.level = std::atoi(token.c_str());
            if (std::getline(iss, token, ','))
                pokeMoves.order = std::atoi(token.c_str());
            

            // std::cout << line << std::endl;
             pokeMovesV.push_back(pokeMoves);

        }

        file.close();
    }
};
std::vector<PokemonMoves> PokemonMoves::pokeMovesV;

class PokemonSpecies {
public:
    int id;
    std::string identifier;
    int generation_id;
    int evolves_from_species_id;
    int evolution_chain_id;
    int color_id;
    int shape_id;
    int habitat_id;
    int gender_rate;
    int capture_rate;
    int base_happiness;
    int is__baby;
    int hatch_counter;
    int has_gender_differences;
    int growth_rate_id;
    int forms_switchable;
    int is_legendary;
    int is_mythical;
    int order;
    int conquest_order;
};

 std::vector<PokemonSpecies> pokeSpeciesV;

void parseAndPrintPokemonSpeciesFile() {
    const char *dbpath = "/share/cs327/pokedex/pokedex/data/csv/";
    const char *filename = "pokemon_species.csv";

    std::string filepath = std::string(dbpath) + filename;

    std::ifstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "Error opening the file" << std::endl;
        return; // Return early if the file can't be opened.
    }

    // std::string header;
    // if (std::getline(file, header)) {
    //     std::cout << header << std::endl;
    // }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        PokemonSpecies pokeSpecies;

        if (std::getline(iss, token, ','))
            pokeSpecies.id = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            pokeSpecies.identifier = token;
        if (std::getline(iss, token, ','))
            pokeSpecies.generation_id = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            pokeSpecies.evolution_chain_id = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            pokeSpecies.color_id = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            pokeSpecies.shape_id = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            pokeSpecies.habitat_id = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            pokeSpecies.gender_rate = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            pokeSpecies.capture_rate = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            pokeSpecies.base_happiness = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            pokeSpecies.is__baby = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            pokeSpecies.hatch_counter = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            pokeSpecies.has_gender_differences = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            pokeSpecies.growth_rate_id = std::atoi(token.c_str());
        if (std::getline(iss, token))
            pokeSpecies.forms_switchable = std::atoi(token.c_str());
         if (std::getline(iss, token, ','))
            pokeSpecies.is_legendary = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            pokeSpecies.is_mythical = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            pokeSpecies.order = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            pokeSpecies.conquest_order = std::atoi(token.c_str());
    

        // std::cout << line << std::endl;
         pokeSpeciesV.push_back(pokeSpecies);

    }

    file.close();
}

void parseAndPrintPokemonSpeciesHomeFile() {
    const char *filename = "pokemon_species.csv";
        // Get the home directory
        const char *homeDir = getenv("HOME");
        if (homeDir == nullptr) {
            std::cerr << "Error: Database not found" << std::endl;
            return;
        }

        // Construct the full file path
        std::string filepath = std::string(homeDir) + "/.poke327/pokedex/pokedex/data/csv/" + filename;

        std::ifstream file(filepath);

        if (!file.is_open()) {
            std::cerr << "Error opening the file" << std::endl;
            return; // Return early if the file can't be opened.
        }

    // std::string header;
    // if (std::getline(file, header)) {
    //     std::cout << header << std::endl;
    // }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        PokemonSpecies pokeSpecies;

        if (std::getline(iss, token, ','))
            pokeSpecies.id = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            pokeSpecies.identifier = token;
        if (std::getline(iss, token, ','))
            pokeSpecies.generation_id = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            pokeSpecies.evolution_chain_id = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            pokeSpecies.color_id = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            pokeSpecies.shape_id = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            pokeSpecies.habitat_id = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            pokeSpecies.gender_rate = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            pokeSpecies.capture_rate = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            pokeSpecies.base_happiness = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            pokeSpecies.is__baby = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            pokeSpecies.hatch_counter = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            pokeSpecies.has_gender_differences = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            pokeSpecies.growth_rate_id = std::atoi(token.c_str());
        if (std::getline(iss, token))
            pokeSpecies.forms_switchable = std::atoi(token.c_str());
         if (std::getline(iss, token, ','))
            pokeSpecies.is_legendary = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            pokeSpecies.is_mythical = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            pokeSpecies.order = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            pokeSpecies.conquest_order = std::atoi(token.c_str());
    

        // std::cout << line << std::endl;
         pokeSpeciesV.push_back(pokeSpecies);

    }

    file.close();
}


class Experience {
public:
   int growth_rate_id;
   int level;
   int experience;

 static std::vector<Experience> experienceV;


//WHYYYYY
void parseAndPrintExperienceFile() {
    const char *dbpath = "/share/cs327/pokedex/pokedex/data/csv/";
    const char *filename = "experience.csv";

    std::string filepath = std::string(dbpath) + filename;

    std::ifstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "Error opening the file" << std::endl;
        return; // Return early if the file can't be opened.
    }

    // std::string header;
    // if (std::getline(file, header)) {
    //     std::cout << header << std::endl;
    // }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;

        Experience experiences;

        if (std::getline(iss, token, ','))
            experiences.growth_rate_id = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            experiences.level = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            experiences.experience = std::atoi(token.c_str());

        // std::cout << line << std::endl;
             experienceV.push_back(experiences);

    }

    file.close();
    }

    void parseAndPrintExperienceHomeFile() {
    const char *filename = "experience.csv";

    const char *homeDir = getenv("HOME");
        if (homeDir == nullptr) {
            std::cerr << "Error: Database not found" << std::endl;
            return;
        }

        // Construct the full file path
        std::string filepath = std::string(homeDir) + "/.poke327/pokedex/pokedex/data/csv/" + filename;

        std::ifstream file(filepath);

        if (!file.is_open()) {
            std::cerr << "Error opening the file" << std::endl;
            return; // Return early if the file can't be opened.
        }

    // std::string header;
    // if (std::getline(file, header)) {
    //     std::cout << header << std::endl;
    // }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;

        Experience experiences;

        if (std::getline(iss, token, ','))
            experiences.growth_rate_id = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            experiences.level = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            experiences.experience = std::atoi(token.c_str());

        // std::cout << line << std::endl;
             experienceV.push_back(experiences);
        
    }

    file.close();
    }
};
std::vector<Experience> Experience::experienceV;


class PokemonStats {
public:
    int pokemon_id;
    int stat_id;
    int base_stat;
    int effort;

    static std::vector<PokemonStats> pokeStatsV;

    void parseAndSavePokemonStatsFile() {
        const char *dbpath = "/share/cs327/pokedex/pokedex/data/csv/";
        const char *filename = "pokemon_stats.csv";

        std::string filepath = std::string(dbpath) + filename;

        std::ifstream file(filepath);

        if (!file.is_open()) {
            std::cerr << "Error opening the file" << std::endl;
            return; // Return early if the file can't be opened.
        }

        // std::string header;
        // if (std::getline(file, header)) {
        //     std::cout << header << std::endl;
        // }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string token;

            PokemonStats pokeStats;

            if (std::getline(iss, token, ','))
                pokeStats.pokemon_id = std::atoi(token.c_str());
            if (std::getline(iss, token, ','))
                pokeStats.stat_id = std::atoi(token.c_str());
            if (std::getline(iss, token, ','))
                pokeStats.base_stat = std::atoi(token.c_str());
            if (std::getline(iss, token, ','))
                pokeStats.effort = std::atoi(token.c_str());

            // std::cout << line << std::endl;
             pokeStatsV.push_back(pokeStats);

        }

        file.close();
    }

void parseAndSavePokemonStatsHomeFile() {
        const char *filename = "pokemon_stats.csv";
        
        const char *homeDir = getenv("HOME");
            if (homeDir == nullptr) {
                std::cerr << "Error: Database not found" << std::endl;
                return;
            }

            // Construct the full file path
            std::string filepath = std::string(homeDir) + "/.poke327/pokedex/pokedex/data/csv/" + filename;

            std::ifstream file(filepath);

            if (!file.is_open()) {
                std::cerr << "Error opening the file" << std::endl;
                return; // Return early if the file can't be opened.
            }

        // std::string header;
        // if (std::getline(file, header)) {
        //     std::cout << header << std::endl;
        // }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string token;

           PokemonStats pokeStats;

            if (std::getline(iss, token, ','))
                pokeStats.pokemon_id = std::atoi(token.c_str());
            if (std::getline(iss, token, ','))
                pokeStats.stat_id = std::atoi(token.c_str());
            if (std::getline(iss, token, ','))
                pokeStats.base_stat = std::atoi(token.c_str());
            if (std::getline(iss, token, ','))
                pokeStats.effort = std::atoi(token.c_str());

            // std::cout << line << std::endl;
             pokeStatsV.push_back(pokeStats);
        }

        file.close();
    }

};
std::vector<PokemonStats> PokemonStats::pokeStatsV;

class PokemonTypes {
public:
    int pokemon_id;
    int type_id;
    int slot;

     static std::vector<PokemonTypes> typesData;

    void parseAndSavePokemonTypesFile() {
        const char *dbpath = "/share/cs327/pokedex/pokedex/data/csv/";
        const char *filename = "pokemon_types.csv";

        std::string filepath = std::string(dbpath) + filename;

        std::ifstream file(filepath);

        if (!file.is_open()) {
            std::cerr << "Error opening the file" << std::endl;
            return; // Return early if the file can't be opened.
        }

        // std::string header;
        // if (std::getline(file, header)) {
        //     std::cout << header << std::endl;
        // }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string token;
             PokemonTypes type;

            if (std::getline(iss, token, ','))
               type.pokemon_id = std::atoi(token.c_str());
            if (std::getline(iss, token, ','))
                type.type_id = std::atoi(token.c_str());
            if (std::getline(iss, token, ','))
                type.slot = std::atoi(token.c_str());

            typesData.push_back(type);
            // std::cout << line << std::endl;
        }

        file.close();
    }

void parseAndSavePokemonTypesHomeFile() {
    const char *filename = "pokemon_types.csv";

    // Get the home directory
    const char *homeDir = getenv("HOME");
    if (homeDir == nullptr) {
        std::cerr << "Error: Database not found" << std::endl;
        return;
    }

    // Construct the full file path
    std::string filepath = std::string(homeDir) + "/.poke327/pokedex/pokedex/data/csv/" + filename;

    std::ifstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "Error opening the file" << std::endl;
        return; // Return early if the file can't be opened.
    }

    // std::string header;
    // if (std::getline(file, header)) {
    //     std::cout << header << std::endl;
    // }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;

        if (std::getline(iss, token, ','))
            pokemon_id = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            type_id = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            slot = std::atoi(token.c_str());

        // std::cout << line << std::endl;
    }

    file.close();
}

};
std::vector<PokemonTypes> PokemonTypes::typesData;

class TypeNames {
public:
    int type_id;
    int local_language_id;
    std::string name;
};

 std::vector<TypeNames> typeNamesV;

void parseAndPrintTypeNamesFile() {
    const char *dbpath = "/share/cs327/pokedex/pokedex/data/csv/";

    const char *filename = "type_names.csv";

    std::string filepath = std::string(dbpath) + filename;

    std::ifstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "Error opening the file" << std::endl;
        return; // Return early if the file can't be opened.
    }

    // std::string header;
    // if (std::getline(file, header)) {
    //     std::cout << header << std::endl;
    // }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        TypeNames typeNames;

        if (std::getline(iss, token, ','))
            typeNames.type_id = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            typeNames.local_language_id = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            typeNames.name = token;
    

        // std::cout << line << std::endl;
         typeNamesV.push_back(typeNames);

    }

    file.close();
}

void parseAndPrintTypeNamesHomeFile() {
    const char *filename = "type_names.csv";

    const char *homeDir = getenv("HOME");
    if (homeDir == nullptr) {
        std::cerr << "Error: Database not found" << std::endl;
        return;
    }

    // Construct the full file path
    std::string filepath = std::string(homeDir) + "/.poke327/pokedex/pokedex/data/csv/" + filename;

    std::ifstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "Error opening the file" << std::endl;
        return; // Return early if the file can't be opened.
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        TypeNames typeNames;

        if (std::getline(iss, token, ','))
            typeNames.type_id = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            typeNames.local_language_id = std::atoi(token.c_str());
        if (std::getline(iss, token, ','))
            typeNames.name = token;
    

        // std::cout << line << std::endl;
         typeNamesV.push_back(typeNames);

    }

    file.close();
}

int getPokemonLevelById(int pokemonId) {
    auto it = std::find_if(PokemonMoves::pokeMovesV.begin(), PokemonMoves::pokeMovesV.end(), [pokemonId](const PokemonMoves& moves) {
        return moves.pokemon_id == pokemonId;
    });

    if (it != PokemonMoves::pokeMovesV.end()) {
        return it->level;
    }

    return 1; 
}


void encounter(Map* map) {
    int encounter = rand() % 100;
    int randomIndex = std::rand() % pokemonV.size();
    Pokemon randomStat = pokemonV[randomIndex];

    if (encounter <= 100 && map->map[pc.position.y][pc.position.x] == ':') {
        randomStat.initializeAttributes();

        int pokemonLevel = getPokemonLevelById(randomStat.id);
        randomStat.levelGet = getPokemonLevelById(randomStat.id);

        mvprintw(25, 0, "Level %d %s", pokemonLevel, randomStat.identifier.c_str());
        mvprintw(26, 0, "IVs: HP=%d, Attack=%d", randomStat.iv_hp, randomStat.iv_attack);
        mvprintw(27, 0, "Shiny: %s", randomStat.is_shiny ? "Yes" : "No");
    }
}


//FOR TESTING IGNORE
void setNewXforPC(int x){
    newXforPC =  x;
}

void setNewYforPC(int y){
    newYforPC = y;
}

int getNewXforPC(){
    return newXforPC;
}

int getNewYforPC(){
    return newYforPC;
}


int main(int argc, char* argv[]) {

        const char *dbpath = "/share/cs327/pokedex/pokedex/data/csv/";
        const char *statsFile = "stats.csv";
        std::string filepathStats = std::string(dbpath) + statsFile;
        std::ifstream fileStats(filepathStats);
        if (!fileStats.is_open()) {
           parseAndPrintStatsHomeFile();
        }else{
              parseAndPrintStatsFile();
        }


        const char *pokemonFile = "pokemon.csv";
        std::string filepathPokemon = std::string(dbpath) + pokemonFile;
        std::ifstream filePokemon(filepathPokemon);
        if (!filePokemon.is_open()) {
           parseAndPrintPokemonHomeFile();
        }else{
            parseAndPrintPokemonFile();
        }


        const char *movesFile = "moves.csv";
        std::string filePathMoves = std::string(dbpath) + movesFile;
        std::ifstream fileMoves(filePathMoves);
        if (!fileMoves.is_open()) {
            parseAndPrintMovesHomeFile();
        }else{
               parseAndPrintMovesFile();
        }

       
        PokemonMoves pokeMoves;
        const char *pokeMovesFile = "pokemon_moves.csv";
        std::string filepathPokeMoves = std::string(dbpath) + pokeMovesFile;
        std::ifstream filePokeMoves(filepathPokeMoves);
        if (!filePokeMoves.is_open()) {
            pokeMoves.parseAndPrintPokemonMovesHomeFile();
        }else{
              pokeMoves.parseAndPrintPokemonMovesFile();
        }
        


        const char *pokeSpeciesFile = "pokemon_species.csv";
        std::string filepathPokeSpecies = std::string(dbpath) + pokeSpeciesFile;
        std::ifstream filepokeSpecies(filepathPokeSpecies);
        if (!filepokeSpecies.is_open()) {
            parseAndPrintPokemonSpeciesHomeFile();
        }else{
            parseAndPrintPokemonSpeciesFile();
        }



        Experience experience;
        const char *experienceFile = "experience.csv";
        std::string filepathExperience = std::string(dbpath) + experienceFile;
        std::ifstream fileExperience(filepathExperience);
        if (!fileExperience.is_open()) {
             experience.parseAndPrintExperienceHomeFile();
        }else{
            experience.parseAndPrintExperienceFile();
        }



        PokemonStats pokeStats;
        const char *pokemonStatsFile = "pokemon_stats.csv";
        std::string filepathPokeStats = std::string(dbpath) + pokemonStatsFile;
        std::ifstream filepokeStats(filepathPokeStats);
        if (!filepokeStats.is_open()) {
           pokeStats.parseAndSavePokemonStatsHomeFile();
        }else{
            pokeStats.parseAndSavePokemonStatsFile();
        }



        PokemonTypes pokeTypes;
        const char *pokeTypesFile = "pokemon_types.csv";
        std::string filepathPokeTypes = std::string(dbpath) + pokeTypesFile;
        std::ifstream filepokeTypes(filepathPokeTypes);
        if (!filepokeTypes.is_open()) {
            pokeTypes.parseAndSavePokemonTypesHomeFile();
        }else{
            pokeTypes.parseAndSavePokemonTypesFile();
        }



        const char *typeNamesFile = "type_names.csv";
        std::string filepathTypeNames = std::string(dbpath) + typeNamesFile;
        std::ifstream fileTypeNames(filepathTypeNames);
        if (!fileTypeNames.is_open()) {
             parseAndPrintTypeNamesHomeFile();
        }else{
             parseAndPrintTypeNamesFile();
        }


    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    start_color();
    init_pair(PLAYER_PAIR, COLOR_RED, COLOR_BLACK);

    srand(time(NULL));
    mvprintw(1, 0, "Loading World...");
    refresh();

    Map (*map)[MAPSY] = (Map (*)[MAPSY])malloc(MAPSX * sizeof(Map[MAPSY]));
    // Check if memory allocation was successful
    if (map == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1; // Exit with an error code
    }

        for (int x = 0; x < MAPSX; x++) {
        for (int y = 0; y < MAPSY; y++) {
            generateMap(&map[x][y]);
        }
    }
   
    //generateMap(&map[currentMapX][currentMapY]);
    int move;
    
    genPathCM(&map[currentMapX][currentMapY], 0, 0);
    pathGenerated[currentMapX][currentMapY] = 1;
    placePCFly(&map[currentMapX][currentMapY]);
     printMap(&map[currentMapX][currentMapY]);
     mvprintw(22, 0, "Please enter a command");

    //  printCSV();



    while ((move = getch()) != 'q') {
         isTrainerThere();
        //  mvprintw(23, 0, "PC location: (%d, %d)", pc.position.x, pc.position.y);
         mvprintw(23, 0, "Current Map: (%d, %d)", currentMapX, currentMapY);
        //  mvprintw(25, 0, "N-S Path Y: %d", snPath.position.x);
        //  mvprintw(26, 0, "E-W Path X: %d", ewPath.position.y);
        //  mvprintw(27, 0, "newX & newY: (%d,%d)", getNewXforPC(), getNewYforPC());

        if(move == '7' || move == 'y'){
            updatePCLocation(&map[currentMapX][currentMapY], move);
            updateHikerLocation(&map[currentMapX][currentMapY]);
            updateRivalLocation(&map[currentMapX][currentMapY]);
            updatePacerLocation(&map[currentMapX][currentMapY]);
            updateWandererLocation(&map[currentMapX][currentMapY]);
            updateExplorerLocation(&map[currentMapX][currentMapY]);
            fflush(stdout);
            usleep(50000);
            printMap(&map[currentMapX][currentMapY]);
            encounter(&map[currentMapX][currentMapY]);
        }else if(move == '8' || move == 'k'){
        updatePCLocation(&map[currentMapX][currentMapY], move);
            updateHikerLocation(&map[currentMapX][currentMapY]);
            updateRivalLocation(&map[currentMapX][currentMapY]);
            updatePacerLocation(&map[currentMapX][currentMapY]);
            updateWandererLocation(&map[currentMapX][currentMapY]);
            updateExplorerLocation(&map[currentMapX][currentMapY]);
            fflush(stdout);
            usleep(50000);
            printMap(&map[currentMapX][currentMapY]);
            encounter(&map[currentMapX][currentMapY]);
       }else if(move == '9' || move == 'u'){
       updatePCLocation(&map[currentMapX][currentMapY], move);
            updateHikerLocation(&map[currentMapX][currentMapY]);
            updateRivalLocation(&map[currentMapX][currentMapY]);
            updatePacerLocation(&map[currentMapX][currentMapY]);
            updateWandererLocation(&map[currentMapX][currentMapY]);
            updateExplorerLocation(&map[currentMapX][currentMapY]);
            fflush(stdout);
            usleep(50000);
            printMap(&map[currentMapX][currentMapY]);
            encounter(&map[currentMapX][currentMapY]);
       }else if(move =='6' || move == 'l'){
        updatePCLocation(&map[currentMapX][currentMapY], move);
            updateHikerLocation(&map[currentMapX][currentMapY]);
            updateRivalLocation(&map[currentMapX][currentMapY]);
            updatePacerLocation(&map[currentMapX][currentMapY]);
            updateWandererLocation(&map[currentMapX][currentMapY]);
            updateExplorerLocation(&map[currentMapX][currentMapY]);
            fflush(stdout);
            usleep(50000);
            printMap(&map[currentMapX][currentMapY]);
            encounter(&map[currentMapX][currentMapY]);
       }else if(move =='3' || move == 'n'){
       updatePCLocation(&map[currentMapX][currentMapY], move);
            updateHikerLocation(&map[currentMapX][currentMapY]);
            updateRivalLocation(&map[currentMapX][currentMapY]);
            updatePacerLocation(&map[currentMapX][currentMapY]);
            updateWandererLocation(&map[currentMapX][currentMapY]);
            updateExplorerLocation(&map[currentMapX][currentMapY]);
            fflush(stdout);
            usleep(50000);
            printMap(&map[currentMapX][currentMapY]);
            encounter(&map[currentMapX][currentMapY]);
       }else if(move =='2' || move == 'j'){
        updatePCLocation(&map[currentMapX][currentMapY], move);
            updateHikerLocation(&map[currentMapX][currentMapY]);
            updateRivalLocation(&map[currentMapX][currentMapY]);
            updatePacerLocation(&map[currentMapX][currentMapY]);
            updateWandererLocation(&map[currentMapX][currentMapY]);
            updateExplorerLocation(&map[currentMapX][currentMapY]);
            fflush(stdout);
            usleep(50000);
            printMap(&map[currentMapX][currentMapY]);
            encounter(&map[currentMapX][currentMapY]);
       }else if(move =='1' || move == 'b'){
        updatePCLocation(&map[currentMapX][currentMapY], move);
            updateHikerLocation(&map[currentMapX][currentMapY]);
            updateRivalLocation(&map[currentMapX][currentMapY]);
            updatePacerLocation(&map[currentMapX][currentMapY]);
            updateWandererLocation(&map[currentMapX][currentMapY]);
            updateExplorerLocation(&map[currentMapX][currentMapY]);
            fflush(stdout);
            usleep(50000);
            printMap(&map[currentMapX][currentMapY]);
            encounter(&map[currentMapX][currentMapY]);
       }else if(move =='4' || move == 'h'){
            updatePCLocation(&map[currentMapX][currentMapY], move);
            updateHikerLocation(&map[currentMapX][currentMapY]);
            updateRivalLocation(&map[currentMapX][currentMapY]);
            updatePacerLocation(&map[currentMapX][currentMapY]);
            updateWandererLocation(&map[currentMapX][currentMapY]);
            updateExplorerLocation(&map[currentMapX][currentMapY]);
            fflush(stdout);
            usleep(50000);
            printMap(&map[currentMapX][currentMapY]);
            encounter(&map[currentMapX][currentMapY]);
       }else if(move == '5' || move =='.' || move == ' '){
            updatePCLocation(&map[currentMapX][currentMapY], move);
            updateHikerLocation(&map[currentMapX][currentMapY]);
            updateRivalLocation(&map[currentMapX][currentMapY]);
            updatePacerLocation(&map[currentMapX][currentMapY]);
            updateWandererLocation(&map[currentMapX][currentMapY]);
            updateExplorerLocation(&map[currentMapX][currentMapY]);
            fflush(stdout);
            usleep(50000);
            printMap(&map[currentMapX][currentMapY]);
            encounter(&map[currentMapX][currentMapY]);
       }else if(move =='t'){
          fflush(stdout);
          usleep(50000);
          printMap(&map[currentMapX][currentMapY]);

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

       }else if(move == '>' && isPConCorM(&map[currentMapX][currentMapY]) == 1){
         mvprintw(22, 0, "Welcome to the PokeCenter!");
       }else if (move == '>' && isPConCorM(&map[currentMapX][currentMapY]) == 2){
        mvprintw(22, 0, "Welcome to the PokeMart!");
       }else if(move == '<' || move == 27){
            updatePCLocation(&map[currentMapX][currentMapY], move);
            fflush(stdout);
            usleep(500000);
            printMap(&map[currentMapX][currentMapY]);

       }else if (pc.position.x == WIDTH - 1) {
                if (currentMapX + 1 < MAPSX) {
                    placePC(1, pc.position.y);
                    currentMapX++;
                    if(pathGenerated[currentMapX][currentMapY] == 0){
                        genPathCM(&map[currentMapX][currentMapY], 0, ewPath.position.y); 
                        pathGenerated[currentMapX][currentMapY] = 1;
                    }
                    printMap(&map[currentMapX][currentMapY]);
            } 
    }else if (pc.position.x <= 0) {
        // Move to the left map
        if (currentMapX - 1 >= 0) {
           placePC(78, pc.position.y);
            currentMapX--;
                 if(pathGenerated[currentMapX][currentMapY] == 0){
                        genPathCM(&map[currentMapX][currentMapY], 0, ewPath.position.y); 
                        pathGenerated[currentMapX][currentMapY] = 1;
                 }
            printMap(&map[currentMapX][currentMapY]);
        } 
    }else if (pc.position.y >= LENGTH - 1) {
        // Move to the right map   
                if (currentMapY + 1 < MAPSY) {
                    placePC(pc.position.x, 1);
                    currentMapY++;
                if(pathGenerated[currentMapX][currentMapY] == 0){
                        genPathCM(&map[currentMapX][currentMapY], snPath.position.x, 0); 
                        pathGenerated[currentMapX][currentMapY] = 1;
                 }
                     
                    printMap(&map[currentMapX][currentMapY]);
                } 
    } else if (pc.position.y <= 0) {
        // Move to the left map
        if (currentMapY - 1 >= 0) {
            placePC(pc.position.x, 19);
            currentMapY--;
            if(pathGenerated[currentMapX][currentMapY] == 0){
                    genPathCM(&map[currentMapX][currentMapY], snPath.position.x, 0); 
                    pathGenerated[currentMapX][currentMapY] = 1;
            }
            printMap(&map[currentMapX][currentMapY]);
        } 
    }else if (move == 'f'){
    int x, y;
        mvprintw(28, 0, "Enter the X coordinate: ");
        refresh();

        char x_input[20];
        int x_index = 0;
        int ch;

        while (1) {
            ch = getch();
            if (ch == '\n') {
                break; // Exit the loop when Enter is pressed
            } else if (ch >= '0' && ch <= '9' && x_index < 19) {
                x_input[x_index++] = ch;
                printw("%c", ch);
            }
        }
        x_input[x_index] = '\0';

        if (sscanf(x_input, "%d", &x) != 1) {
            mvprintw(23, 0, "Invalid X coordinate. Please enter a valid integer.");
            refresh();
            break; // Exit the function if the input is invalid
        }

    mvprintw(29, 0, "Enter the Y coordinate: ");
        refresh();

        char y_input[20];
        int y_index = 0;

        while (1) {
            ch = getch();
            if (ch == '\n') {
                break; // Exit the loop when Enter is pressed
            } else if (ch >= '0' && ch <= '9' && y_index < 19) {
                y_input[y_index++] = ch;
                printw("%c", ch);
            }
        }
        y_input[y_index] = '\0';

        // Convert the Y coordinate string to an integer
        if (sscanf(y_input, "%d", &y) != 1) {
            mvprintw(25, 0, "Invalid Y coordinate. Please enter a valid integer.");
            refresh();
            break; // Exit the function if the input is invalid
        }

        currentMapX = x;
        currentMapY = y;

        genPathCM(&map[currentMapX][currentMapY], 0, 0); 
        placePCFly(&map[currentMapX][currentMapY]);
        printMap(&map[currentMapX][currentMapY]);

        mvprintw(30, 0, "Moved to x=%d and y=%d", x, y);
        refresh();
    }
        }
        free(map);
        endwin();
        
        return 0;
    }