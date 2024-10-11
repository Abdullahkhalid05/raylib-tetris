#include <stdlib.h>
#include <raylib.h>
#include <stdbool.h>

#define COLS 10
#define ROWS 20
#define BLOCK_SIZE 30
#define NAME "TETRIS"
#define SPEED 1
#define BLOCK_TYPES 7

void drawGridLines() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            DrawRectangleLines(j * BLOCK_SIZE, i * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, BLACK);
        }
    }
}

struct Piece {
    int width;
    int height;
    Color color;
    int xCoords[4];
    int yCoords[4];
};

void drawBlock(int x, int y, struct Piece piece) {
    DrawRectangle(x + piece.xCoords[0] * BLOCK_SIZE, y + piece.yCoords[0] * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, piece.color);
    DrawRectangle(x + piece.xCoords[1] * BLOCK_SIZE, y + piece.yCoords[1] * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, piece.color);
    DrawRectangle(x + piece.xCoords[2] * BLOCK_SIZE, y + piece.yCoords[2] * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, piece.color);
    DrawRectangle(x + piece.xCoords[3] * BLOCK_SIZE, y + piece.yCoords[3] * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, piece.color);
}
struct Piece randPiece() {
    struct Piece pieces[BLOCK_TYPES] = {
        {
            .width = 1,
            .height = 4,
            .color = RED,
            .xCoords = {0, 0, 0, 0},
            .yCoords = {0, 1, 2, 3},
        },
        {
            .width = 2,
            .height = 2,
            .color = YELLOW,
            .xCoords = {0, 0, 1, 1},
            .yCoords = {0, 1, 1, 0},
        },
        {
            .width = 3,
            .height = 2,
            .color = BLUE,
            .xCoords = {0, 1, 2, 1},
            .yCoords = {0, 0, 0, 1},
        },
        {
            .width = 3,
            .height = 2,
            .color = GREEN,
            .xCoords = {0, 1, 1, 2},
            .yCoords = {1, 0, 1, 0},
        },
        {
            .width = 3,
            .height = 2,
            .color = PURPLE,
            .xCoords = {0, 1, 1, 2},
            .yCoords = {0, 0, 1, 1},
        },
    {
            .width = 2,
            .height = 3,
            .color = ORANGE,
            .xCoords = {0, 0, 0, 1},
            .yCoords = {0, 1, 2, 2},
        },
        {
            .width = 2,
            .height = 3,
            .color = BROWN,
            .xCoords = {1, 1, 1, 0},
            .yCoords = {0, 1, 2, 2},
        },
    };
    return pieces[rand()%BLOCK_TYPES];
}
//static to intiallize 0
static int boardState[ROWS][COLS];

int isHittingAnotherBlock(int x, int y, struct Piece piece) {
    return 
            boardState[y + piece.yCoords[0] + 1][x + piece.xCoords[0]] ||
            boardState[y + piece.yCoords[1] + 1][x + piece.xCoords[1]] ||
            boardState[y + piece.yCoords[2] + 1][x + piece.xCoords[2]] ||
            boardState[y + piece.yCoords[3] + 1][x + piece.xCoords[3]] ;
}
void removeLine(int line) {
    // remove the line
    for (int x = 0; x < COLS; x++) {
        boardState[0][x] = 0 ;
    }
    // move the up blocks one step down
    for (int y = line; y > 0; y--) {
        for(int x = 0 ; x < COLS ; x++ ) {
            boardState[y][x] = boardState[y - 1][x];
        }
    }
}


void checkAndClearFullLines() {
  for (int y = 0; y < ROWS; y++) {
    int isFull = true;
    for (int x = 0; x < COLS; x++) {
      if (boardState[y][x] == 0) {
        isFull = false;
        break;
      }
    }
    if (isFull) {
      removeLine(y);
    }
  }
}

bool gameOver = false;
struct Piece rotateBlock(struct Piece piece, int rotation) {
    struct Piece tempBlock;
    tempBlock.color = piece.color;
    int ir = (int) rotation /90;
    switch (ir) {
    case 0:
        tempBlock.width = piece.width;
        tempBlock.height = piece.height;
        for(int i = 0; i < 4; i++) {
            tempBlock.xCoords[i] = piece.xCoords[i];
            tempBlock.yCoords[i] = piece.yCoords[i];
        }
        break;
    case 2:
        tempBlock.width = piece.height;
        tempBlock.height = piece.width;
        for(int i = 0; i < 4; i++) {
            tempBlock.xCoords[i] = piece.yCoords[i];
            tempBlock.yCoords[i] = piece.xCoords[i];
        }
        break;
    case 3:
        tempBlock.width = piece.width;
        tempBlock.height = piece.height;
        for(int i = 0; i < 4; i++) {
            tempBlock.xCoords[i] = -piece.xCoords[i];
            tempBlock.yCoords[i] = piece.yCoords[i];
        }
        break;
    case 1:
        tempBlock.width = piece.width;
        tempBlock.height = piece.height;
        for(int i = 0; i < 4; i++) {
            tempBlock.xCoords[i] = piece.xCoords[i];
            tempBlock.yCoords[i] = -piece.yCoords[i];
        }
        break;
    }
    return tempBlock;
}

void printBoard(void) {
    InitWindow(COLS * BLOCK_SIZE, ROWS * BLOCK_SIZE, NAME);
    SetTargetFPS(30);
    int x =  BLOCK_SIZE;
    int dy = 0;
    int y = 0;
  
    struct Piece currentPiece = randPiece();
    int rotation = 0;
    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(WHITE);
        // draw loop
        if(!gameOver){
            drawBlock(x, y, currentPiece);
            drawGridLines();
            for (int i = 0; i < ROWS; i++) {
                for (int j = 0; j < COLS; j++) {
                    if(boardState[i][j]){
                        DrawRectangle(j * BLOCK_SIZE, i * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, RED);
                    }
            }
            
            }
        }else {
            DrawText("GAME OVER", 40, 100, 24, RED);
        }
        EndDrawing();
        // game logic

        dy += SPEED;
        if (dy % BLOCK_SIZE == 0) {
            y += BLOCK_SIZE;
        }

        int ix = (int)x/BLOCK_SIZE;
        int iy = (int)y/BLOCK_SIZE;
        if (isHittingAnotherBlock(ix, iy, currentPiece) || iy >= ROWS - currentPiece.height) {
            boardState[iy + currentPiece.yCoords[0]][ix + currentPiece.xCoords[0]] = 1; 
            boardState[iy + currentPiece.yCoords[1]][ix + currentPiece.xCoords[1]] = 1; 
            boardState[iy + currentPiece.yCoords[2]][ix + currentPiece.xCoords[2]] = 1; 
            boardState[iy + currentPiece.yCoords[3]][ix + currentPiece.xCoords[3]] = 1; 
             checkAndClearFullLines();  
            
            currentPiece = randPiece();
            if (iy == 0) {
                gameOver = true;
            }
            y = 0;
            x = rand()%COLS * BLOCK_SIZE;
        }
        int key = GetKeyPressed();
        switch (key) {
        case KEY_LEFT:
            if (x > 0) {
                x -= BLOCK_SIZE;
            }
            break;

        case KEY_RIGHT:
            if (x < BLOCK_SIZE * (COLS - currentPiece.width)) {
                x += BLOCK_SIZE;
            }
            break;
        case KEY_DOWN:
            y += BLOCK_SIZE;
            break;

        case KEY_SPACE:
            rotation += 90;
            if(rotation == 360) rotation = 0;
            currentPiece = rotateBlock(currentPiece, rotation);
            break;
        }
    }
    CloseWindow();
}

int main(void) {
    printBoard();
    return 0;
}
// NOTE: Don not use any chatgpt
