#include "simulateHeuristics.h"

// Fast random move maker. Chance of finding a square doesn't decrease with each move.
int randomMoveMaker(uint8_t squares[9]){
    srand(time(NULL));

    int zeros[9] = { 0 };
    int zeroCount = 0;

    for (int i = 0; i < 9; i++) {
        if (squares[i] == 0) {
            zeros[zeroCount] = i;
            zeroCount++;
        }
    }
    // Assume that there are empty squares.
    // Because that check is already done in simulate().
    
    int randomIndex = rand() % zeroCount;
    int index = zeros[randomIndex];

    return index;
}

// Simulate heuristics
int simulate()
{
    
    // The table will contain the average win from "the opening".
    int oHeuristics[9] = { 0 };
    int winCount = 0;
    int drawCount = 0;
    int lossCount = 0;

    // starting square state
    for (int i = 0; i < 9; i++) 
    {
        boardState board;
        resetBoard(&board);
        changeBoard(&board, i);

        boardState newBoard = board;

        // Simulate a thousand games. -- Doing this without the rand() optimization takes forever.
        for (int j = 0; j < 1000; j++)
        {
            //printf("Starting game %d\n", j + 1);
            // Play board until a win condition is met.
            while (!newBoard.wincondition && newBoard.filledSquares < 9) 
            {
                int nextMove = randomMoveMaker(newBoard.square);
                Sleep(1); // Sleep for 1 ms to avoid race condition.

                //printf("Inserted move : %d\n", nextMove);
                changeBoard(&newBoard, nextMove);
            }
            // showBoard(newBoard.square);
            // printf("\nGame %d ends\n", j + 1);

            // Still necessary to check, because we don't know if it's a wincondition or a draw.
            // Nine squares can be filled with a wincondition, which is why we check this before
            // checking the filled squares.
            if (newBoard.wincondition == 1) {
                if (newBoard.turn == 1) { // Bit flips on the last move.
                    // If O wins, it flips to X, and we count it as a win.  
                    winCount++;
                    newBoard = board;
                    //printf("Game ends in a win!\n");
                    continue;
                }
                lossCount++;
                newBoard = board;
                //printf("Game ends in a loss!\n");
                continue;
            }
            // Unreachable unless it's a draw.
            drawCount++;
            newBoard = board;
            //printf("Game ends in a draw!\n");
            continue;

        }
        // Average the wins, losses and draws for the current square
        oHeuristics[i] = (winCount - lossCount) + (drawCount/2);

        printf("Current square: %d\n", i);
        printf("Wins: %d, Draws: %d, Losses: %d\n", winCount, drawCount, lossCount);
        
        // Reset the counts for the next square
        winCount = 0;
        lossCount = 0;
        drawCount = 0;
    }

    // Then, finally, print the heuristics.
    printf("Heuristics for each square:\n");
    for (int i = 0; i < 9; i++) {
        if (i % 3 == 0) {
            printf("\n");
        }
        printf(" [ %d ] ", oHeuristics[i]);
    }

    return 0;
}

// I get a heuristic table that looks like this:
// [ 319 ]  [ 221 ]  [ 470 ]
// [ 302 ]  [ 659 ]  [ 267 ]
// [ 355 ]  [ 319 ]  [ 500 ]
/* Output
Current square: 0
Wins: 614, Draws: 61, Losses: 325
Current square: 1
Wins: 492, Draws: 158, Losses: 350
Current square: 2
Wins: 567, Draws: 224, Losses: 209
Current square: 3
Wins: 453, Draws: 264, Losses: 283
Current square: 4
Wins: 711, Draws: 158, Losses: 131
Current square: 5
Wins: 438, Draws: 261, Losses: 301
Current square: 6
Wins: 505, Draws: 230, Losses: 265
Current square: 7
Wins: 518, Draws: 189, Losses: 293
Current square: 8
Wins: 673, Draws: 103, Losses: 224
*/