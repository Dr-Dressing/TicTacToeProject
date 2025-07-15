// @author: Dr-Dressing
// @date : 15-07-2025
// @version : 1.0

#include "negamax.h"

// Technically a "playAI".
// This function makes the AI play on the board.
// @param board: The board to play on.
void initAI(boardState *board)
{
    // Deciding depth of 2 for now.
    // randomMoveMaker(board);
    uint8_t squareResult;


    negamax(board, 9 - board->filledSquares, &squareResult); // 9 because that's the max depth.
    // printf("Move played, %d\n", squareResult);
    changeBoard(board, squareResult);
}

// Heuristic based on 1000 simulated games.
// These are overkill.
int heuristics[9] = 
{ 
    319, 221, 470,
    302, 659, 267,
    355, 319, 500 
};

// Terminal eval.
// eval is (supposed to be) a heuristic evaluation.
// @param board: The board to evaluate.
int eval(boardState *board){
    // Thanks for suggesting the subtraction, toanth.
    if (board->wincondition == WIN) return __INT32_MAX__ - board->filledSquares; // If more squares are filled, the lower the score to find a faster win.
    if (board->wincondition == DRAW) return 0; // draw condiiton, better than a loss.

    return 0; // If this somehow happens, something is wrong in the code.
}

// Negamax is the decision-making algorithm for the AI. I think that Alpha-beta pruning is overkill here.
// @param board: The board to play on (including temporary boards).
// @param depth: The "depth" to search to. In this case, it's the number of remaining squares. Although I don't really use it.
// @param squareCheck: The final square. It's played on the board (temps inclusive).
// @return: The initial negamax returns a square index. The rest return an evaluation.
int negamax(boardState *board, uint8_t depth, uint8_t *squareCheck)
{

    int bestMove = -__INT32_MAX__;

    moveList legalList = { .moves = { 0 }, .size = 0 };
    legalList = legalMoves(board, &legalList);


    // Technically not necessary, because a win/draw is determined through search anyway.
    if (legalList.size == 0) return eval(board);

    for (int i = 0; i < legalList.size; i++){
        
        int value = -__INT32_MAX__;

        boardState newboard;
        memcpy(&newboard, board, sizeof(*board));

        changeBoard(&newboard, legalList.moves[i]);

        if (newboard.wincondition == WIN){
            *squareCheck = legalList.moves[i];
            return __INT32_MAX__ - newboard.filledSquares; // See eval().
        }

        if (newboard.wincondition == DRAW) return 0;

        value = __max(value, -negamax(&newboard, depth - 1, squareCheck));

        if (bestMove <= value) 
        {
            *squareCheck = legalList.moves[i]; 
            bestMove = value;
        }
        
    }
    return bestMove;
}