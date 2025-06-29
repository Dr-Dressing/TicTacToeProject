#include "game.h"

void resetBoard(boardState *board)
{
    //memset(board, 0, sizeof(board));
    memset(board->square, 0, sizeof(board->square));
    board->wincondition = 0;
    board->filledSquares = 0;
    board->turn = 0;
    board->illegal = 0;
}

void showBoard(int square[9])
{
    for (int i = 0; i < 9; i++) {
        if (i % 3 == 0) printf("\n"); // Ideally to show a 3x3 row of "squares".
        if (square[i] == 0) printf(" [ - ] ");
        if (square[i] == 1) printf(" [ X ] ");
        if (square[i] == 2) printf(" [ O ] ");
    }
}

int gameLoop(boardState *board)
{
    while (!board->wincondition)
    {
        int squareIndex; // Written square index. This will be used in the placement logic.
        
        printf("\e[1;1H\e[2J"); // Clear console.

        if (board->illegal) { printf("ILLEGAL INPUT - MOVE RETRACTED"); board->illegal = 0; }
        
        printf("\nTurn-val : %d\n", board->turn);
        showBoard(board->square);
        printf("\nInsert X/O on the index of your square (0-8).\n");
        scanf("%d", &squareIndex); // Highly unsafe, I suppose. But it's fine for this case. 
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

int init()
{
    struct boardState board;
    
    resetBoard(&board);
    gameLoop(&board);
    
    return 0;
}