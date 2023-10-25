#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

/*-----GLOBAL VARIABLES---------*/
#define LENGTH 21
#define WIDTH 80
#define NUM_MAPS 5
char map[LENGTH][WIDTH];
char *maps[NUM_MAPS][LENGTH][WIDTH];
int currentMap = 0;

char pastMap[LENGTH][WIDTH];


/*-----FUNCTION PROTOTYPES---------*/
void generateMap();
void printMap();
void fillMapGrass(int x, int y);
void generateTerrains(int x, int y);
void generateBorder(int x, int y);
void genPathCM(int x, int y);
void initMaps();
void switchMap(char input);
void saveMap();




int main(int argc, char *argv[]) {
srand(time(NULL));
   initMaps();
   generateMap();
   printMap();
   
   char input;
   while (1) {
    printf("Where would you like to go ");
      scanf("%c", &input);
      switchMap(input);
      printMap();
   }
   return 0;
}

void initMaps() {
   for (int i = 0; i < NUM_MAPS; i++) {
      for (int j = 0; j < LENGTH; j++) {
         *maps[i][j] = &map[j][0];
      }
   }
}

void switchMap(char input) {
   
   if (input == 'n') {
      currentMap = (currentMap + 1) % NUM_MAPS;
      generateMap();
   }else if(input =='s'){
    currentMap = (currentMap - 1) % NUM_MAPS;
      generateMap();
   }else if(input =='w'){
     currentMap = (currentMap - 2) % NUM_MAPS;
      generateMap();
   }else if(input == 'e'){
     currentMap = (currentMap +2) % NUM_MAPS;
      generateMap();
   }else{
    exit(1);
   }
    
}

void saveMap(){
    for(int i = 0; i < LENGTH; i++){
        for(int j = 0; j < WIDTH; j++){
            pastMap[i][j] = map[i][j];
        }
    }
}

void generateMap(){
     int x, y;
    fillMapGrass(x,y); //fills the empty spaces of the map with grass
    generateTerrains(x, y); //randomly places the different terrains around the map
    generateBorder(x,y); //generates the border around everything
    genPathCM(x,y); //creates the path and the PokeCenter and PokeMart
    pastMap[y][x];
}

//-------------FUNCTIONS BELOW-----------------------------------------------------------------------------------------------------------------------------------------------------------------------

// print the map
void printMap(){
    int x;
    int y;
    for (y = 0; y < LENGTH; y++) {
        for (x = 0; x < WIDTH; x++) {
            printf("%c", map[y][x]);
        }
        printf("\n");
    }
}



void fillMapGrass(int x, int y){
for (y = 0; y < LENGTH; y++) {
        for (x = 0; x < WIDTH; x++) {
                map[y][x] = '.';
        }
    }
}


void generateTerrains(int x, int y){
 //placing the tall grass randomly
    int numTallGrass = 2 + rand() % 10;  // adds 2-10 regions of tall grass fulfilling the requirement of at least 2 regions

    for (int i = 0; i < numTallGrass; i++) {
       int  tallGrassWidth = 4 + rand() % 10;  // 4-13
        int tallGrassHeight = 4 + rand() % 6;  // 4-9
        x = 1 + rand() % (WIDTH - tallGrassWidth - 1);
        y = 1 + rand() % (LENGTH - tallGrassHeight - 1);

        if(x < 80 || y < 21) {
        for (int j = y; j < y + tallGrassHeight; j++) {
            for (int k = x; k < x + tallGrassWidth; k++) {
                map[j][k] = ':';
            }
        }
        }
    }

    //placing  trees
    int numTrees = 2 + rand() % 5;  // adds 2-6 regions of tall grass 
 
    for (int i = 0; i < numTrees; i++) {
       int  treeWidth = 4 + rand() % 10;  // 4-13
       int  treeHeight = 4 + rand() % 6;  // 4-9
        x = 1 + rand() % (WIDTH - numTrees - 1);
        y = 1 + rand() % (LENGTH - treeHeight - 1);


    if(x < 80 && x > 1 && y > 1 && y < 21) {
        for (int j = y; j < y + treeHeight; j++) {
            for (int k = x; k < x + treeWidth; k++) {
                map[j][k] = '^';
            }
        }
    }

    }



    //placing water
    int numWater = 1 + rand() % 5;  // adds 2-6 regions of tall grass 
 
    for (int i = 0; i < numWater; i++) {
        int waterWidth = 4 + rand() % 10;  // 4-13
        int waterHeight = 4 + rand() % 6;  // 4-9
        x = 1 + rand() % (WIDTH - numWater - 1);
        y = 1 + rand() % (LENGTH - waterHeight - 1);

    if(x < 80 || y < 21) {
        for (int j = y; j < y + waterHeight; j++) {
            for (int k = x; k < x + waterWidth; k++) {
                map[j][k] = '~';
            }
        }
    }

    }

    //placing boulders + mountains
    int numRocks = 1 + rand() % 5;  // adds 1-3, i didn't want as many of these
 

    for (int i = 0; i < numRocks; i++) {
        int rockWidth = 4 + rand() % 9;  // 4-13
        int rockHeight = 4 + rand() % 6;  // 4-9


        x = 1 + rand() % (WIDTH - numRocks - 1);
        y = 1 + rand() % (LENGTH - rockHeight - 1);

       if(x < 80 || y < 21) { //so it doesn't go through the border
        for (int j = y; j < y + rockHeight; j++) {
            for (int k = x; k < x + rockWidth; k++) {
                map[j][k] = '%';
            }
        }

       }

    }
}



void generateBorder(int x, int y){
    for (x = 0; x < WIDTH; x++) {
        map[0][x] = '%'; //starting at the left corner
        map[LENGTH - 1][x] = '%';
    }
    for (y = 0; y < LENGTH; y++) {
        map[y][0] = '%'; 
        map[y][WIDTH - 1] = '%';
    }
}


void genPathCM(int x, int y){
    // making the path that goes from east to west
    y = rand() % (LENGTH - 3) + 1;  //so it doesnt touch the top border
    for (x = 0; x < WIDTH; x++) {
             map[y][x] = '#';
       
    }

   
    //places the C in a random place right next to the path
    x = rand() & (WIDTH - 3);
    if(y < 21 && y > 1 && x < 80 && x > 1){
        map[y + 1][x] = 'C';
    }else{
        map[y - 1][x] = 'C';
    }

    //places the M on the map next to the C to be easier
    if(x < 80 && x > 1 && y < 21 && y > 1){
        map[y+1][x+1] = 'M';
    }else if(map[y][x] == '#'){
        map[y][x+1] = 'M';
    }else{
        map[y-1][x-1] = 'M';
    }
    
     
    // making the path that goes from north to south
    x = rand() % (WIDTH - 3); 
    for (y = 0; y < LENGTH; y++) {
        map[y][x] = '#';
    }
}