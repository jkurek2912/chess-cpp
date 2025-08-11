#ifndef BOARD_H
#define BOARD_H

#include <array>
#include "pieces.h"
#include <vector>
#include "move.h"
#define NUM_ROWS 8
#define NUM_COLS 8

class Board
{
public:
    std::array<std::array<Piece, 8>, 8> squares;
    Color sideToMove;

    Board();

    void print() const;

    inline int squareIndex(int row, int col)
    {
        return row * 8 + col;
    }

    inline int rowFromIndex(int index)
    {
        return index / 8;
    }

    inline int colFromIndex(int index)
    {
        return index % 8;
    }

    std::vector<Move> generatePawnMoves();
};

#endif
