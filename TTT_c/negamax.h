// @author: Dr-Dressing
// @date : 15-07-2025
// @version : 1.0

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>



typedef struct boardState
{
    uint8_t square[9];
    uint8_t wincondition; // Non-zero suggests loss or draw, with AI playing (X).
    uint8_t turn;
    uint8_t filledSquares;
    uint8_t illegal;
} boardState;


// Just a fancy list, because making it with int pointers yields undefined behavior.
// This is a list of legal moves, which is used by the AI to make decisions.
typedef struct moveList
{
    int moves[9]; 
    int size;
} moveList;

// I was told to enum, as opposed to bitboard wins, losses and draws.
enum {
    NONE = 0,
    WIN = 1,
    DRAW = 2,
};

moveList legalMoves(boardState *board, moveList *list);
void changeBoard(boardState *board, uint8_t index);
void initAI(boardState *board);
int eval(boardState *board);
int negamax(boardState *board, uint8_t depth, uint8_t *squareCheck);
