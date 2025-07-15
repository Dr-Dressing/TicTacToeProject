// For C++, I'm not a huge fan using classes. So, I'll go with structs instead.

#include <stdint.h>

// Defs
enum {
    NONE,
    X,
    O,
    WIN = 1,
    DRAW = 2
};

typedef struct boardState {
    uint8_t winCondition;
    uint8_t turn;
    uint8_t filledSquares;
    uint8_t illegal;
} boardState;

typedef struct moveList {
    uint8_t moves[9];
    uint8_t size;
} moveList;

class CGame {
    private:
        boardState mainBoard;
        moveList currentLegals;

    public:
        CGame();
        ~CGame();

        int init();
        void resetBoard();
        void showBoard(uint8_t square[9]);
};


void changeBoard();
