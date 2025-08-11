#include "board.h"
#include <iostream>

Board::Board() : sideToMove(WHITE) {
    // Set up starting position
    squares = {{
        {WR, WN, WB, WQ, WK, WB, WN, WR},
        {WP, WP, WP, WP, WP, WP, WP, WP},
        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
        {BP, BP, BP, BP, BP, BP, BP, BP},
        {BR, BN, BB, BQ, BK, BB, BN, BR}
    }};
}

void Board::print() const {
    for (int row = 0; row < 8; ++row) {
        std::cout << 8 - row << " ";
        for (int col = 0; col < 8; ++col) {
            char c = '.';
            switch (squares[row][col]) {
                case WP: c = 'P'; break;
                case WN: c = 'N'; break;
                case WB: c = 'B'; break;
                case WR: c = 'R'; break;
                case WQ: c = 'Q'; break;
                case WK: c = 'K'; break;
                case BP: c = 'p'; break;
                case BN: c = 'n'; break;
                case BB: c = 'b'; break;
                case BR: c = 'r'; break;
                case BQ: c = 'q'; break;
                case BK: c = 'k'; break;
                default: break;
            }
            std::cout << c << " ";
        }
        std::cout << "\n";
    }
    std::cout << "  a b c d e f g h\n";
}
