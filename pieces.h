#ifndef PIECES_H
#define PIECES_H

enum Piece {
    EMPTY, WP, WN, WB, WR, WQ, WK,
    BP, BN, BB, BR, BQ, BK
};

enum Color {
    WHITE, BLACK, NO_COLOR
};

inline bool isWhite(Piece p) {
    return p >= WP && p <= WK;
}

inline bool isBlack(Piece p) {
    return p >= BP && p <= BK;
}

#endif
