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

    inline bool pieceFoundAndTurnToMove(Color sideToMove, Piece p)
    {
        return (sideToMove == WHITE && isWhite(p)) || (sideToMove == BLACK && isBlack(p));
    }

    bool inBounds(int row, int col)
    {
        return row >= 0 && row < NUM_ROWS && col >= 0 && col < NUM_COLS;
    }

    std::vector<Move> generatePawnMoves();

    std::vector<Move> generateKnightMoves();

    std::vector<Move> generateBishopMoves();

    std::vector<Move> generateRookMoves();

    std::vector<Move> generateQueenMoves();

    std::vector<Move> generateKingMoves();

    void applyMove(const Move &move);
};

#endif
