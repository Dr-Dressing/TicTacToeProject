#include "game.h"


// resetBoard is a trivial function that resets the board state.
// @param board: The address of the board to reset.
void resetBoard(boardState *board)
{
    //memset(board, 0, sizeof(board));
    memset(board->square, 0, sizeof(board->square));
    board->wincondition = 0;
    board->filledSquares = 0;
    board->turn = 0;
    board->illegal = 0;
}

// changeBoard is is a trivial function that changes the board state.
// @param board: The address of the board to change.
// @param index: The index of the square to change.
void changeBoard(boardState *board, uint8_t index)
{
    if (board->square[index] || index > 8 || index < 0) { board->illegal = 1; return; }
    if (board->turn) { board->square[index] = 1; } else { board->square[index] = 2; }
    if (board->wincondition) { board->illegal = 1; return; } 
    board->illegal = 0;
    board->turn ^= 1;
    board->filledSquares++;

    // Win condition check.
    for (uint8_t i = 0; i < 3; i++) {

        // provided the offsets return non-zero, there has been reached a win-condition.
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
            // No break here, as further searches could find a win condition.
        }
    }
    return;
}

// showBoard prints the board to the console in the format
// [ - ] for empty squares, [ X ] for X's, and [ O ] for O's.
// @param square: The board to print.
void showBoard(uint8_t square[9])
{
    // Testing grounds.
    #ifdef TEST
    printf("\n\n");
    #else
    printf("\e[1;1H\e[2J"); // Clear console.
    #endif


    for (uint8_t i = 0; i < 9; i++) {
        if (i % 3 == 0) printf("\n"); // Ideally to show a 3x3 row of "squares".
        if (square[i] == 0) printf(" [ - ] ");
        if (square[i] == 1) printf(" [ X ] ");
        if (square[i] == 2) printf(" [ O ] ");
    }
}

// gameLoop is exactly that.
// @param board: The address of the main board to play on. 
// (Subsequent boards are copies of this board, so the AI can play on a copy of the board.)
// @return: Returns 0 -- It could be used for error handling, but for this project, 
// it isn't exactly necessary.
int gameLoop(boardState *board)
{
    while (!board->wincondition)
    {
        int squareIndex; // Written square index. This is the input.

        printf("\nTurn-val : %d\n", board->turn);
        showBoard(board->square);
        printf("\nInsert X/O on the index of your square (0-8).\n");
        scanf("%d", &squareIndex); // Highly unsafe, I suppose. But it's fine for this case. 
        // In this case, I sacrifice safety for simplicity.

        changeBoard(board, squareIndex);
        if (board->wincondition) { break; }
        else { initAI(board); }
    }
    printf("\e[1;1H\e[2J");
    printf("\nTurn-val : %d\n", board->turn);
    showBoard(board->square);

    if (board->wincondition == 2) { printf("\n IT'S A DRAW!"); return 0; }
    (board->turn ^= 1) ? printf("\n X Wins!") : printf("\n O Wins!");

    return 0;
}

// Empty board upon initialization.
int init()
{
    struct boardState board;
    
    resetBoard(&board);
    gameLoop(&board);
    
    return 0;
}