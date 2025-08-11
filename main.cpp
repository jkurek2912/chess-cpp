#include <iostream>
#include "board.h"

int main() {
    Board board;
    board.print();

    auto pawnMoves = board.generatePawnMoves();
    std::cout << "Pawn moves count: " << pawnMoves.size() << "\n";

    if (!pawnMoves.empty()) {
        std::cout << "Applying first pawn move...\n";
        board.applyMove(pawnMoves[0]);
        board.print();
    }

    return 0;
}
