#include "minimax.h"


void initAI(boardState *board)
{
    // Deciding depth of 2 for now.
    // randomMoveMaker(board);
    int squareResult;
    minimax(board, 2, &squareResult, true); // Depth here is the equivalent of ply in chess.
    changeBoard(board, squareResult);
    // Max depth being the max number of moves, when AI gets
    // control.
}


int eval(boardState *board, int *squareCheck){
    if (board->turn ^= 1 && board->wincondition == 1) return 1000;
    if (board->turn ^= 1 && board->wincondition == 2) return 0; // draw condiiton, better than loss.
    if (!board->turn && board->wincondition == 1) return -1000; // Immediate threat.

    return 0; // No immediate threats.
}

int minimax(boardState *board, int depth, int *squareCheck, uint8_t maximizingPlayer)
{
    int value; 
    if (depth == 0 || board->wincondition) return eval(board, squareCheck);
    value = -999;
    for (int i = 0; i < 9; i++){
        boardState newboard;
        memcpy(&newboard, board, sizeof(*board));
        if (!newboard.square[i])
        {
            changeBoard(&newboard, i);
            int compareValue = -minimax(&newboard, depth - 1, squareCheck, maximizingPlayer ^1);
            if (value < compareValue){
                value = compareValue;
                *squareCheck = i; // Latest square is the first depth search: FILO.
            }
        }
    }
    return value;
}

