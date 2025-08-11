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

            if ((sideToMove == WHITE && p == WP) || (sideToMove == BLACK && p == BP))
            {
                int newRow = r + direction;

                if (inBounds(newRow, c) && squares[newRow][c] == EMPTY)
                {
                    moves.emplace_back(squareIndex(r, c), squareIndex(newRow, c));

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
                    if (inBounds(newRow, captureCol))
                    {
                        Piece target = squares[newRow][captureCol];
                        if (target != EMPTY && ((sideToMove == WHITE && isBlack(target)) || (sideToMove == BLACK && isWhite(target))))
                        {
                            moves.emplace_back(squareIndex(r, c), squareIndex(newRow, captureCol));
                        }
                    }
                }

                // TODO: add promotion and en passant
            }
        }
    }
    return moves;
}

std::vector<Move> Board::generateKnightMoves()
{
    std::vector<Move> moves;
    const std::vector<std::pair<int, int>> knightOffset = {{-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}, {2, -1}, {2, 1}};

    for (int r = 0; r < NUM_ROWS; r++)
    {
        for (int c = 0; c < NUM_COLS; c++)
        {
            Piece p = squares[r][c];
            if ((sideToMove == WHITE && p == WN) || (sideToMove == BLACK && p == BN))
            {
                for (auto [nr, nc] : knightOffset)
                {
                    int newRow = r + nr;
                    int newCol = c + nc;
                    if (inBounds(newRow, newCol))
                    {
                        Piece target = squares[newRow][newCol];
                        if (target == EMPTY || (sideToMove == WHITE && isBlack(target) || (sideToMove == BLACK && isWhite(target))))
                        {
                            moves.emplace_back(squareIndex(r, c), squareIndex(newRow, newCol));
                        }
                    }
                }
            }
        }
    }
    return moves;
}

std::vector<Move> Board::generateBishopMoves()
{
    std::vector<Move> moves;
    const std::vector<std::pair<int, int>> directions = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
    for (int r = 0; r < NUM_ROWS; r++)
    {
        for (int c = 0; c < NUM_COLS; c++)
        {
            Piece p = squares[r][c];
            if ((sideToMove == WHITE && p == WB) || (sideToMove == BLACK && p == BB))
            {
                for (auto [dr, dc] : directions)
                {
                    int newRow = r + dr;
                    int newCol = c + dc;
                    while (inBounds(newRow, newCol))
                    {
                        Piece target = squares[newRow][newCol];
                        if (target == EMPTY)
                        {
                            moves.emplace_back(squareIndex(r, c), squareIndex(newRow, newCol));
                        }
                        else
                        {
                            if ((sideToMove == WHITE && isBlack(target)) || (sideToMove == BLACK && isWhite(target)))
                            {
                                moves.emplace_back(squareIndex(r, c), squareIndex(newRow, newCol));
                            }
                            break;
                        }
                        newRow += dr;
                        newCol += dc;
                    }
                }
            }
        }
    }
    return moves;
}

std::vector<Move> Board::generateRookMoves()
{
    std::vector<Move> moves;
    const std::vector<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    for (int r = 0; r < NUM_ROWS; r++)
    {
        for (int c = 0; c < NUM_COLS; c++)
        {
            Piece p = squares[r][c];
            if ((sideToMove == WHITE && p == WR) || (sideToMove == BLACK && p == BR))
            {
                for (auto [dr, dc] : directions)
                {
                    int newRow = r + dr;
                    int newCol = c + dc;
                    while (inBounds(newRow, newCol))
                    {
                        Piece target = squares[newRow][newCol];
                        if (target == EMPTY) {
                            moves.emplace_back(squareIndex(r, c), squareIndex(newRow, newCol));
                        } else {
                            if ((sideToMove == WHITE && isBlack(target)) || (sideToMove == BLACK && isWhite(target))) {
                                moves.emplace_back(squareIndex(r, c), squareIndex(newRow, newCol));
                            }
                            break;
                        }
                        newRow += dr;
                        newCol += dc;
                    }
                }
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