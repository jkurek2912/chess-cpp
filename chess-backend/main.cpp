#include <iostream>
#include "board.h"

int main() {
    Board board;
    board.print();

    auto moves = board.generateKnightMoves();
    std::cout << "(Piece) moves count: " << moves.size() << "\n";

    if (!moves.empty()) {
        std::cout << "Applying first move...\n";
        board.applyMove(moves[0]);
        board.print();
    }

    return 0;
}
