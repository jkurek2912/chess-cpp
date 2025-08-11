#ifndef BOARD_H
#define BOARD_H

#include <array>
#include "pieces.h"

class Board {
public: 
    std::array<std::array<Piece, 8>, 8> squares;
    Color sideToMove;

    Board();

    void print() const;
};

#endif
