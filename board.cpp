#include "board.h"
#include <iostream>

Board::Board() : sideToMove(WHITE)
{
    squares = {{{BR, BN, BB, BQ, BK, BB, BN, BR},
                {BP, BP, BP, BP, BP, BP, BP, BP},
                {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
                {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
                {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
                {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
                {WP, WP, WP, WP, WP, WP, WP, WP},
                {WR, WN, WB, WQ, WK, WB, WN, WR}}};
}

void Board::print() const
{
    for (int row = 0; row < 8; ++row)
    {
        std::cout << 8 - row << " ";
        for (int col = 0; col < 8; ++col)
        {
            char c = '.';
            switch (squares[row][col])
            {
            case WP:
                c = 'P';
                break;
            case WN:
                c = 'N';
                break;
            case WB:
                c = 'B';
                break;
            case WR:
                c = 'R';
                break;
            case WQ:
                c = 'Q';
                break;
            case WK:
                c = 'K';
                break;
            case BP:
                c = 'p';
                break;
            case BN:
                c = 'n';
                break;
            case BB:
                c = 'b';
                break;
            case BR:
                c = 'r';
                break;
            case BQ:
                c = 'q';
                break;
            case BK:
                c = 'k';
                break;
            default:
                break;
            }
            std::cout << c << " ";
        }
        std::cout << "\n";
    }
    std::cout << "  a b c d e f g h\n";
}

std::vector<Move> Board::generatePawnMoves()
{
    std::vector<Move> moves;
    int direction = (sideToMove == WHITE) ? -1 : 1;

    for (int r = 0; r < NUM_ROWS; r++)
    {
        for (int c = 0; c < NUM_COLS; c++)
        {
            Piece p = squares[r][c];
            std::cout << (int)p << " ";

            if ((sideToMove == WHITE && p == WP) || (sideToMove == BLACK && p == BP))
            {
                int nextRow = r + direction;

                if (nextRow >= 0 && nextRow < NUM_ROWS && squares[nextRow][c] == EMPTY)
                {
                    moves.emplace_back(squareIndex(r, c), squareIndex(nextRow, c));

                    // two steps (only on starting rank)
                    bool isStartRank = (sideToMove == WHITE && r == 6) || (sideToMove == BLACK && r == 1);
                    int twoStepRow = r + 2 * direction;
                    if (isStartRank && squares[twoStepRow][c] == EMPTY)
                    {
                        moves.emplace_back(squareIndex(r, c), squareIndex(twoStepRow, c));
                    }
                }

                // captures
                for (int dc : {-1, 1})
                {
                    int captureCol = c + dc;
                    if (captureCol >= 0 && captureCol < NUM_COLS && nextRow >= 0 && nextRow < NUM_ROWS)
                    {
                        Piece target = squares[nextRow][captureCol];
                        if (target != EMPTY && ((sideToMove == WHITE && isBlack(target)) || (sideToMove == BLACK && isWhite(target))))
                        {
                            moves.emplace_back(squareIndex(r, c), squareIndex(nextRow, captureCol));
                        }
                    }
                }

                // TODO: add promotion and en passant
            }
        }
    }
    return moves;
}

void Board::applyMove(const Move &move)
{
    int fromRow = rowFromIndex(move.from);
    int fromCol = colFromIndex(move.from);
    int toRow = rowFromIndex(move.to);
    int toCol = colFromIndex(move.to);

    Piece movingPiece = squares[fromRow][fromCol];
    squares[toRow][toCol] = movingPiece;
    squares[fromRow][fromCol] = EMPTY;

    // TODO: castling, promotion, en passant

    sideToMove = (sideToMove == WHITE) ? BLACK : WHITE;
}