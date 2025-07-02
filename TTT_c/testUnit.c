#include "testUnit.h"
#define TEST 

// I'm not testing showBoard, as it prints to the console.
// - And it's trivial.

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

    printf("Test 3 complete\n");
}

// Simple minimax testing. Immediate win condition, and loss prevention.
void test4()
{
    // Setting up a board with an immediate win condition.
    boardState board;
    resetBoard(&board); // Reset is also a kind of init.
    changeBoard(&board, 0);
    changeBoard(&board, 1);
    changeBoard(&board, 3);
    changeBoard(&board, 4);
    
    // O X -
    // O X -
    // - - - 
    // with O to move.

    initAI(&board); // AI should play square 6.
    showBoard(board.square);

    assert(board.square[6] == 2); // AI plays O.
    assert(board.illegal == 0); // No illegal moves allowed.


    // Immediate loss prevention.
    resetBoard(&board);
    changeBoard(&board, 0);
    changeBoard(&board, 1);
    changeBoard(&board, 3);

    initAI(&board); // Should play square 6, to prevent an immediate loss.

    // O X -
    // O - -
    // - - - 
    // with X to move.

    showBoard(board.square);
    printf("\n");
    assert(board.square[6] == 1);
    assert(board.illegal == 0);

    printf("Test 4 complete\n");

}

void testAll()
{
    test1();
    test2();
    test3();
    test4();
    printf("All tests complete\n");
    return;
}