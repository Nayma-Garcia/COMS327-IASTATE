#include <iostream>
#include <ncurses.h>
#include <vector>
#include <cstdlib>
#include <ctime>

const int boardSize = 10;

struct Ship {
    int size;
    int hits;
};

struct Cell {
    bool hasShip;
    bool isHit;
};

typedef std::vector<std::vector<Cell>> Board;

int currentPlayer = 0;

void initializeBoard(Board &board) {
    for (int i = 0; i < boardSize; ++i) {
        board.push_back(std::vector<Cell>(boardSize, {false, false}));
    }
}

void printBoard(const Board &board) {
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if (board[i][j].isHit) {
                if (board[i][j].hasShip) {
                    printw("X ");
                } else {
                    printw("o ");
                }
            } else {
                printw(". ");
            }
        }
        printw("\n");
    }
}

bool isValidMove(const Board &board, int row, int col) {
    return row >= 0 && row < boardSize && col >= 0 && col < boardSize && !board[row][col].isHit;
}

void placeShip(Board &board, int row, int col, int size, bool horizontal) {
    if (horizontal) {
        for (int i = 0; i < size; ++i) {
            board[row][col + i].hasShip = true;
        }
    } else {
        for (int i = 0; i < size; ++i) {
            board[row + i][col].hasShip = true;
        }
    }
}

bool isGameOver(const Board &board) {
    for (int i = 0; i < boardSize; ++i) {
        for (int j = 0; j < boardSize; ++j) {
            if (board[i][j].hasShip && !board[i][j].isHit) {
                return false;
            }
        }
    }
    return true;
}

int main() {
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    timeout(100);

    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);

    srand(time(nullptr));

    Board playerBoard, computerBoard;
    initializeBoard(playerBoard);
    initializeBoard(computerBoard);

  placeShip(playerBoard, rand() % 6, rand() % 6, 3, rand() % 2 == 0);
  placeShip(playerBoard, rand() % 7, rand() % 7, 4, rand() % 2 == 0);

    // Place random ships on the computer board
    placeShip(computerBoard, rand() % 6, rand() % 6, 3, rand() % 2 == 0);
    placeShip(computerBoard, rand() % 7, rand() % 7, 4, rand() % 2 == 0);

    int row = 0, col = 0;

    while (!isGameOver(playerBoard) && !isGameOver(computerBoard)) {
        clear();

        printw("Player's Board:\n");
        printBoard(playerBoard);
        printw("\nComputer's Board:\n");
        printBoard(computerBoard);

        printw("Press 'q' to quit.\n");

        while (currentPlayer == 0) {
            int ch = getch();
            if (ch == 'q') {
                break;
            }

            int newRow = row, newCol = col;

            // Handle arrow key input for movement
            switch (ch) {
                case KEY_UP:
                    newRow--;
                    break;
                case KEY_DOWN:
                    newRow++;
                    break;
                case KEY_LEFT:
                    newCol--;
                    break;
                case KEY_RIGHT:
                    newCol++;  
                    break;
                case '\n':
                    // Execute the move if the user presses Enter
                    if (isValidMove(computerBoard, newRow, newCol)) {
                        computerBoard[newRow][newCol].isHit = true;
                        currentPlayer = 1;
                        refresh();
                    }
                    break;
            }

            // Ensure the new position is valid
            if (newRow >= 0 && newRow < boardSize && newCol >= 0 && newCol < boardSize) {
                row = newRow;
                col = newCol;
            }
        }


        refresh();

        // Computer's move (you can modify this part for a smarter AI)
        int computerRow, computerCol;
        do {
            computerRow = rand() % boardSize;
            computerCol = rand() % boardSize;
        } while (!isValidMove(playerBoard, computerRow, computerCol));

        playerBoard[computerRow][computerCol].isHit = true;

        refresh();
        currentPlayer = 0;
    }

    if (isGameOver(playerBoard)) {
        napms(1000);
        printw("Congratulations! You won!\n");
        napms(1000);
    } else {
        printw("Sorry, you lost. Better luck next time!\n");
    }

    printw("Press any key to exit.");
    getch();
    napms(5000);
    endwin();
    return 0;
}