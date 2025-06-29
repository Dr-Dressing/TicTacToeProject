#include "testUnit.h"

// I'm not testing showBoard, as it prints to the console.
// - And it's trivial.

// Simple one-way hashing algorithm for comparing boards.
int minihash(boardState board){
    int hash = 0;
    for (int i = 0; i < 9; i++){
        hash += board.square[i] * (i + 1);
        hash += board.turn * 10;
        hash += board.filledSquares * 10;
        hash += board.wincondition * 10;
        hash += board.illegal * 10;
    }
    return hash;
}

// resetBoard (simple) test.
void test1(){
    boardState board;
    
    board.filledSquares = 1;
    board.square[0] = 1;
    board.turn = 1;

    resetBoard(&board);
    for (int i = 0; i < 9; i++){
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
    
    int firstHash = minihash(board);

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
    assert(board.square[0] == 2); // Human plays O by default.
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
    for (int i = 0; i < 3; i++) {
        changeBoard(&board, i*3);
        changeBoard(&board, (i*3)+1); // Final move will result in an illegal move.
    }
    assert(board.illegal == 1);
    assert(board.wincondition == 1);
    resetBoard(&board);
    assert(board.wincondition == 0); 
    // Potential reset problems. Idk how you'd get through the first test without resetting the board properly.
    
    // Second column win condition.
    for (int i = 0; i < 3; i++) {
        changeBoard(&board, i*3 + 1);
        changeBoard(&board, (i*3));
    }

    assert(board.wincondition == 1);
    resetBoard(&board);

    // Third column win condition.
    for (int i = 0; i < 3; i++) {
        changeBoard(&board, i*3 + 2);
        changeBoard(&board, (i*3));
    }
    assert(board.wincondition == 1);
    resetBoard(&board);

    // Row win conditions.
    for (int i = 0; i < 3; i++)
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

    printf("Test 3 complete\n");
}

// TODO Potential minimax test.

void testAll()
{
    test1();
    test2();
    test3();
    printf("All tests complete\n");
    return;
}