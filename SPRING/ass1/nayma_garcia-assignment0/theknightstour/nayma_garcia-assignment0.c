#include <stdio.h>

int knightSolved(int, int);
void printSolution();
int moveX(int x, int path);
int moveY(int y, int path);
int validMove(int x, int y, int nextPath);



int board[5][5] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25};

int main(){
    printSolution();
    int xPath;
    int yPath;
    
    xPath++;
    yPath++;
    knightSolved(xPath, yPath);
   // printSolution();

    


    return 0;
}


int moveX(int x, int path){
    if(path == 0){
        x -= 1;
    }else if(path == 1){
        x += 1;
    }else if(path == 2){
        x += 2;
    }else if(path == 3){
        x += 2;
    }else if(path == 4){
        x += 1;
    }else if(path == 5){
        x -= 1;
    }else if(path == 6){
        x -= 2;
    }else if(path == 7){
        x -= 2;
    }

//making sure it doesnt go out of bounds
    if(x < 0 || x > 7){
        return -1;
    }

    return x;
    
}

int moveY(int y, int path){
    if(path == 0){
        y += 2;
    }else if(path == 1){
        y += 2;
    }else if(path == 2){
        y += 1;
    }else if(path == 3){
        y -= 1;
    }else if(path == 4){
        y -= 2;
    }else if(path == 5){
        y -= 2;
    }else if(path == 6){
        y -= 1;
    }else if(path == 7){
        y += 1;
    }

    if(y < 0 || y > 7){
        return -1;
    }

    return y;

}

int knightSolved(int x, int y){
    //upate the path #
    int path = 0;
    path++;
    x = 5;
    y = 5;
    
    //while it hasnt gone through all the paths and x and y are still on the grid
    while(path < 7 && x < 6 && y < 6){
        moveX(x, path);
        moveY(y, path);
        printSolution();
    }
        return 0;
}

void printSolution(){
    for(int i = 0; i < 5; i++){
        for(int j = 0;j < 5; j++){
            if(board[i][j] < 10){
                printf(" ");
            }
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
    printf("\n\n\n");
}