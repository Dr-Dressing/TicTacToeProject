#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <time.h>

typedef struct boardState
{
    int square[9];
    uint8_t wincondition; // Non-zero suggests loss or draw, with AI playing (X).
    uint8_t turn;
    uint8_t filledSquares;
    int illegal;
} boardState;

#define true 1
#define false 0


void changeBoard(boardState *board, int index);
void initAI(boardState *board);
int minimax(boardState *board, int depth, int *squareCheck, uint8_t maximizingPlayer);
int eval(boardState *board, int *squareCheck);
