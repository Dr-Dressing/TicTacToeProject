#include "minimax.h"

void changeBoard(boardState *board, int index)
{
    if (board->square[index] || index > 8 || index < 0) { board->illegal = 1; return; }
    if (board->turn) { board->square[index] = 1; } else { board->square[index] = 2; }
    board->turn ^= 1;
    board->filledSquares++;

    // Win condition check.
    for (uint8_t i = 0; i < 3; i++) {

        // provided the offsets return non-zero, there has been reached a win-condition.
        // TODO: Write tests... :(
        uint8_t offsetVertical = board->square[i] & board->square[i+3] & board->square[i+6];
        uint8_t diagonal1 = board->square[0] & board->square[4] & board->square[8]; 
        uint8_t diagonal2 = board->square[2] & board->square[4] & board->square[6]; 
        uint8_t row = i*3;
        uint8_t offsetHorizontal = board->square[row] & board->square[row + 1] & board->square[row + 2];
        
        if (offsetVertical || offsetHorizontal || diagonal1 || diagonal2) {
            board->wincondition ^= 1;
            break;
        } else if (board->filledSquares == 9) {
            board->wincondition = 2; 
            // No break here, as further searches can find a win condition.
        }
    }
    return;
}


void initAI(boardState *board)
{
    // Deciding depth of 3 for now.
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

