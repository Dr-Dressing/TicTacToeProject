#include "minimax.h"


void initAI(boardState *board)
{
    // Deciding depth of 2 for now.
    // randomMoveMaker(board);
    uint8_t squareResult;

    minimax(board, 8, &squareResult, 1); // Depth here is the equivalent of ply in chess.
    changeBoard(board, squareResult);
    // Max depth being the max number of moves, when AI gets
    // control.
}

int heuristic[9] = 
{ 
    1, 1, 1, 
    1, 1, 1, 
    1, 1, 1
};

// Terminal eval.
// eval is (supposed to be) a heuristic evaluation.
// @param board: The board to evaluate.
// @param squareCheck: The final square. It's played on the board.
// @param maximizingPlayer: The player to evaluate for. 1 for AI, 0 for opponent.
int eval(boardState *board, uint8_t *squareCheck, uint8_t maximizingPlayer){
    if (maximizingPlayer && board->wincondition == 1) return 99999; // win condition.
    if (maximizingPlayer && board->wincondition == 2) return 0; // draw condiiton, better than loss.
    if (!maximizingPlayer && board->wincondition == 1) return -99999; // loss condition.
    
    // Final depth evaluation.
    for (int i = 0; i < 9; i++){
        boardState boardCopy;
        memcpy(&boardCopy, board, sizeof(*board));
        if (!board->square[i]){
            changeBoard(&boardCopy, i);
            if (boardCopy.wincondition == 1) {
                *squareCheck = i; // If AI can win, return square.
                return 1000 * boardCopy.filledSquares;
            } 
            // Return heurestic instead.
            return 1000 * heuristic[i];
        }
    }
    
    return 0;
}

// minimax here is partially inspired on the psudeocode from http://en.wikipedia.org/wiki/Minimax
// I'm planning to change it to negamax, because right now, I'm using attributes of both minimax and negamax. :(
// @param board: The board to play on (including temporary boards).
// @param depth: The depth (ply) to search to.
// @param squareCheck: The final square. It's played on the board (temps inclusive).
int minimax(boardState *board, uint8_t depth, uint8_t *squareCheck, uint8_t maximizingPlayer)
{
    int value;
    int originalValue;
    if (depth == 0 || board->wincondition) return eval(board, squareCheck, maximizingPlayer);
    if (maximizingPlayer) { value = -__INT32_MAX__; } else { value = __INT32_MAX__; }
    for (int i = 0; i < 9; i++){
        boardState newboard;
        memcpy(&newboard, board, sizeof(*board));
        if (!newboard.square[i])
        {
            uint8_t newSquare = 0; // Not to be confused with the squareCheck.
            // This makes sure the original square is not overwritten by an illegal square.

            changeBoard(&newboard, i);

            
            maximizingPlayer ^= 1;
            int compareValue = -minimax(&newboard, depth - 1, &newSquare, maximizingPlayer);
            maximizingPlayer ^= 1; // Undo player change.
            
            // Two maximizingPlayer checks might be slower, but it avoids code duplication.
            if (maximizingPlayer) {
                originalValue = value;
                value = __max(value, compareValue);
                if (value != originalValue) *squareCheck = i; // New square if value changes.
            } 
            else {
                originalValue = value;
                value = __min(value, compareValue);
                if (value != originalValue) *squareCheck = i;
            }
        }
    }
    return value;
}

