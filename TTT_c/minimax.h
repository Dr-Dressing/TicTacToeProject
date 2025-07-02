#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <time.h>


typedef struct boardState
{
    uint8_t square[9];
    uint8_t wincondition; // Non-zero suggests loss or draw, with AI playing (X).
    uint8_t turn;
    uint8_t filledSquares;
    uint8_t illegal;
} boardState;



void changeBoard(boardState *board, uint8_t index);
void initAI(boardState *board);
int eval(boardState *board, uint8_t *squareCheck, uint8_t maximizingPlayer);
int minimax(boardState *board, uint8_t depth, uint8_t *squareCheck, uint8_t maximizingPlayer);
