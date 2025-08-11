#ifndef MOVE_H
#define MOVE_H

enum PieceType {
    NONE = 0,
    PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING
};

struct Move {
    int from;
    int to;
    PieceType promotion;

    Move(int f = -1, int t = -1, PieceType promo = NONE) 
        : from(f), to(t), promotion(promo) {}
};

#endif
