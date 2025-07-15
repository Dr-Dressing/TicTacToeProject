#include "testUnit.h"
#define TEST 

// I'm not testing showBoard, as it prints to the console.
// - And it's trivial.


// HELPER FUNCTIONS

// Simple one-way hashing algorithm for comparing boards.
int minihash(boardState board){
    int hash = 0;
    for (uint8_t i = 0; i < 9; i++){
        hash += board.square[i] * (i + 1);
        hash += board.turn * 10 << 5;
        hash += board.filledSquares * 10 >> 2;
        hash += board.wincondition * 10 << 1;
        hash += board.illegal * 10;
    }
    return hash;
}

// This is a perft inspired by https://www.chessprogramming.org/Perft
// This makes sure that the correct number of legal moves are generated
int perft(int depth, boardState *board){
    
    boardState tempBoard = *board;
    moveList list;

    memcpy(&tempBoard, board, sizeof(*board));
    int legals = 0;
    
    legalMoves(&tempBoard, &list);
    legals += list.size; // Number of leafs in this
    
    if (depth > 1){
        legals = 0;
        for (int i = 0; i < list.size; i++){
            // For each legal move, we make a copy of the board and change it.
            memcpy(&tempBoard, board, sizeof(*board));
            changeBoard(&tempBoard, list.moves[i]);
            
            
            if (tempBoard.illegal) continue;
            legals += perft(depth - 1, &tempBoard);
        }
    }

    //printf("Legals %d\n", legals);
    return legals;
}


// ACTUAL TESTS


// resetBoard (simple) test.
void test1(){
    boardState board;
    
    board.filledSquares = 1;
    for (uint8_t i = 0; i < 9; i++){
        board.square[i] = 1;
    }
    board.turn = 1;

    resetBoard(&board);
    for (uint8_t i = 0; i < 9; i++){
        assert(board.square[i] == 0);
    }
    assert(board.filledSquares == 0);
    assert(board.turn == 0);
    assert(board.wincondition == 0);
    assert(board.illegal == 0);
    assert(minihash(board) == 0);

    printf("Test 1 complete\n");
}

// changeBoard test.
void test2(){
    boardState board;
    resetBoard(&board);
    
    uint8_t firstHash = minihash(board);

    // Test illegal move.
    changeBoard(&board, 10);
    assert(board.illegal == 1);
    assert(board.turn == 0); // Turn should not change.
    assert(minihash(board) != firstHash);
    // Illegal flag set. Boards are not equal.
    // This is because illegal flag contributes
    // to the AI's decision-making.

    // Test valid move.
    changeBoard(&board, 0);
    assert(board.illegal == 0); // Reset illegal flag.
    assert(board.square[0] == 2); // Human plays O by default. The rules don't dictate otherwise, but for now, AI plays X.
    assert(board.turn == 1);
    assert(board.filledSquares == 1);
    assert(minihash(board) != 0);

    // Test another valid move. For instance, that turns switch correctly.
    changeBoard(&board, 1);
    assert(board.square[1] == 1); // AI plays X.
    assert(board.turn == 0);
    assert(board.filledSquares == 2);
    
    printf("Test 2 complete\n");
}

// Test win/draw condition.
void test3()
{

    // There is probably a more efficient way to do this in *one* for-loop, but this works too.
    boardState board;
    resetBoard(&board);
    
    // First column win condition.
    for (uint8_t i = 0; i < 3; i++) {
        changeBoard(&board, i*3);
        changeBoard(&board, (i*3)+1); // Final move will result in an illegal move.
    }
    assert(board.illegal == 1);
    assert(board.wincondition == 1);
    resetBoard(&board);
    assert(board.wincondition == 0); 
    // Potential reset problems. Idk how you'd get through the first test without resetting the board properly.
    
    // Second column win condition.
    for (uint8_t i = 0; i < 3; i++) {
        changeBoard(&board, i*3 + 1);
        changeBoard(&board, (i*3));
    }

    assert(board.wincondition == 1);
    resetBoard(&board);

    // Third column win condition.
    for (uint8_t i = 0; i < 3; i++) {
        changeBoard(&board, i*3 + 2);
        changeBoard(&board, (i*3));
    }
    assert(board.wincondition == 1);
    resetBoard(&board);

    // Row win conditions.
    for (uint8_t i = 0; i < 3; i++)
    {
        changeBoard(&board, i*3);
        changeBoard(&board, (i*3 + 3) % 9);
        changeBoard(&board, i*3 + 1);
        changeBoard(&board, (i*3 + 4) % 9);
        changeBoard(&board, i*3 + 2);
        assert(board.wincondition == 1);
        resetBoard(&board);
    }
    
    // Diagonal 1 win conditions. The diagonal that goes [\]
    changeBoard(&board, 0);
    changeBoard(&board, 3);
    changeBoard(&board, 4);
    changeBoard(&board, 6);
    changeBoard(&board, 8);

    assert(board.wincondition == 1);
    resetBoard(&board);

    // Diagonal 2 win conditions. The diagonal that goes [/]
    changeBoard(&board, 2);
    changeBoard(&board, 3);
    changeBoard(&board, 4);
    changeBoard(&board, 5);
    changeBoard(&board, 6);

    assert(board.wincondition == 1);

    // Draw condition.
    // O X O
    // X X O
    // O O X
    resetBoard(&board);
    changeBoard(&board, 0);
    changeBoard(&board, 1);
    changeBoard(&board, 2);
    changeBoard(&board, 3);
    changeBoard(&board, 5);
    changeBoard(&board, 4);
    changeBoard(&board, 6);
    changeBoard(&board, 8);
    changeBoard(&board, 7);

    assert(board.wincondition == 2); // Draw condition.
    assert(board.filledSquares == 9); // All squares filled.

    printf("Test 3 complete\n");
}

// perft testing the legal move generator.
void test4(){

    // These perfts were verified with this engine
    // https://github.com/toanth/motors
    // Thank you to toanth for suggesting it.

    boardState board;
    
    resetBoard(&board);
    
    int legalMoves = perft(1, &board);
    assert(legalMoves == 9);
    
    legalMoves = perft(2, &board);
    assert(legalMoves == 72);

    legalMoves = perft(3, &board);
    assert(legalMoves == 504);

    legalMoves = perft(4, &board);
    assert(legalMoves == 3024);

    legalMoves = perft(5, &board);
    assert(legalMoves == 15120);

    legalMoves = perft(6, &board);
    assert(legalMoves == 54720);

    legalMoves = perft(7, &board);
    assert(legalMoves == 148176);  

    legalMoves = perft(8, &board);
    assert(legalMoves == 200448);

    legalMoves = perft(9, &board);
    assert(legalMoves == 127872);



    printf("Test 4 complete\n");
}

// Testing win in one move.
void test5(){
    boardState board;
    resetBoard(&board);

    changeBoard(&board, 0);
    changeBoard(&board, 1);
    changeBoard(&board, 3);
    changeBoard(&board, 4);
    
    // O X -
    // O X -
    // - - -

    initAI(&board);
    // showBoard(board.square);
    assert(board.wincondition == 1);

    resetBoard(&board);

    changeBoard(&board, 0);
    changeBoard(&board, 1);
    changeBoard(&board, 3);
    changeBoard(&board, 4);
    changeBoard(&board, 2);

    initAI(&board); // AI should play square 6.
    // showBoard(board.square);
    assert(board.wincondition == 1);

    printf("Test 5 complete\n");
}

// Simulated games between AI and AI.
void test6()
{
    // Setting up a board with an immediate win condition.
    boardState board;

    // It's a little slow, so 10 games should suffice.
    for (int i = 0; i < 10; i++){
        resetBoard(&board);
        while (!board.wincondition)
        {
            initAI(&board);
            usleep(50); // Otherwise I get a race condition. :(
        }
        // showBoard(board.square);
        // printf("Game %d\n", i + 1);
        assert(board.wincondition == DRAW);
    }

    printf("Test 6 complete\n");

}

void testAll()
{
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
    printf("All tests complete\n");
    return;
}