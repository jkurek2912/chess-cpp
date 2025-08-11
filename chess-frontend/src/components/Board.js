import React, { useState, useEffect } from "react";
import "./Board.css";

const pieceSymbols = {
  WP: "♙",
  WN: "♘",
  WB: "♗",
  WR: "♖",
  WQ: "♕",
  WK: "♔",
  BP: "♟",
  BN: "♞",
  BB: "♝",
  BR: "♜",
  BQ: "♛",
  BK: "♚",
  EMPTY: ""
};

const initialBoard = [
  ["BR", "BN", "BB", "BQ", "BK", "BB", "BN", "BR"],
  ["BP", "BP", "BP", "BP", "BP", "BP", "BP", "BP"],
  ["EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY"],
  ["EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY"],
  ["EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY"],
  ["EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY"],
  ["WP", "WP", "WP", "WP", "WP", "WP", "WP", "WP"],
  ["WR", "WN", "WB", "WQ", "WK", "WB", "WN", "WR"]
];

function isWhitePiece(piece) {
  return piece.startsWith("W");
}
function isBlackPiece(piece) {
  return piece.startsWith("B");
}
function isEmpty(piece) {
  return piece === "EMPTY";
}

// Helper: inside board bounds
function inBounds(r, c) {
  return r >= 0 && r < 8 && c >= 0 && c < 8;
}

export default function Board() {
  const [board, setBoard] = useState(initialBoard);
  const [draggedFrom, setDraggedFrom] = useState(null);
  const [sideToMove, setSideToMove] = useState("W");
  const [validMoves, setValidMoves] = useState(new Set()); // set of "fromRow-fromCol-toRow-toCol"

  // Generate all valid moves for the current side
  useEffect(() => {
    setValidMoves(generateValidMoves(board, sideToMove));
  }, [board, sideToMove]);

  // Check if piece belongs to current player
  function canDragPiece(piece) {
    if (sideToMove === "W") return isWhitePiece(piece);
    return isBlackPiece(piece);
  }

  // Handle drag start
  function handleDragStart(e, row, col) {
    if (!canDragPiece(board[row][col])) {
      e.preventDefault();
      return;
    }
    setDraggedFrom({ row, col });
    e.dataTransfer.effectAllowed = "move";
  }

  // Allow dropping
  function handleDragOver(e) {
    e.preventDefault();
    e.dataTransfer.dropEffect = "move";
  }

  // Handle drop: only if move is valid
  function handleDrop(e, toRow, toCol) {
    e.preventDefault();
    if (!draggedFrom) return;

    const { row: fromRow, col: fromCol } = draggedFrom;
    const moveKey = `${fromRow}-${fromCol}-${toRow}-${toCol}`;

    if (!validMoves.has(moveKey)) {
      // Invalid move, ignore
      setDraggedFrom(null);
      return;
    }

    // Make the move
    const newBoard = board.map(r => r.slice());
    newBoard[toRow][toCol] = newBoard[fromRow][fromCol];
    newBoard[fromRow][fromCol] = "EMPTY";

    setBoard(newBoard);
    setSideToMove(sideToMove === "W" ? "B" : "W");
    setDraggedFrom(null);
  }

  // Generate valid moves for all pieces of current side
  function generateValidMoves(board, side) {
    const moves = new Set();

    for (let r = 0; r < 8; r++) {
      for (let c = 0; c < 8; c++) {
        const piece = board[r][c];
        if (
          (side === "W" && isWhitePiece(piece)) ||
          (side === "B" && isBlackPiece(piece))
        ) {
          const pieceMoves = generatePieceMoves(board, r, c, piece, side);
          pieceMoves.forEach(({ toRow, toCol }) =>
            moves.add(`${r}-${c}-${toRow}-${toCol}`)
          );
        }
      }
    }
    return moves;
  }

  // Generate moves for a single piece at (r,c)
  function generatePieceMoves(board, r, c, piece, side) {
    switch (piece[1]) {
      case "P":
        return generatePawnMoves(board, r, c, side);
      case "N":
        return generateKnightMoves(board, r, c, side);
      case "B":
        return generateSlidingMoves(board, r, c, side, [
          [-1, -1],
          [-1, 1],
          [1, -1],
          [1, 1]
        ]);
      case "R":
        return generateSlidingMoves(board, r, c, side, [
          [-1, 0],
          [1, 0],
          [0, -1],
          [0, 1]
        ]);
      case "Q":
        return generateSlidingMoves(board, r, c, side, [
          [-1, -1],
          [-1, 1],
          [1, -1],
          [1, 1],
          [-1, 0],
          [1, 0],
          [0, -1],
          [0, 1]
        ]);
      case "K":
        return generateKingMoves(board, r, c, side);
      default:
        return [];
    }
  }

  // Pawn moves (simple, no en passant, no promotion)
  function generatePawnMoves(board, r, c, side) {
    const moves = [];
    const dir = side === "W" ? -1 : 1;
    const startRow = side === "W" ? 6 : 1;

    // Forward 1
    const fwdR = r + dir;
    if (inBounds(fwdR, c) && isEmpty(board[fwdR][c])) {
      moves.push({ toRow: fwdR, toCol: c });

      // Forward 2 from start
      if (r === startRow) {
        const fwd2R = r + 2 * dir;
        if (inBounds(fwd2R, c) && isEmpty(board[fwd2R][c])) {
          moves.push({ toRow: fwd2R, toCol: c });
        }
      }
    }

    // Captures
    for (let dc of [-1, 1]) {
      const capR = r + dir;
      const capC = c + dc;
      if (
        inBounds(capR, capC) &&
        !isEmpty(board[capR][capC]) &&
        ((side === "W" && isBlackPiece(board[capR][capC])) ||
          (side === "B" && isWhitePiece(board[capR][capC])))
      ) {
        moves.push({ toRow: capR, toCol: capC });
      }
    }

    return moves;
  }

  // Knight moves
  function generateKnightMoves(board, r, c, side) {
    const moves = [];
    const knightOffsets = [
      [-2, -1],
      [-2, 1],
      [-1, -2],
      [-1, 2],
      [1, -2],
      [1, 2],
      [2, -1],
      [2, 1]
    ];

    for (const [dr, dc] of knightOffsets) {
      const nr = r + dr;
      const nc = c + dc;
      if (inBounds(nr, nc)) {
        const target = board[nr][nc];
        if (
          isEmpty(target) ||
          (side === "W" && isBlackPiece(target)) ||
          (side === "B" && isWhitePiece(target))
        ) {
          moves.push({ toRow: nr, toCol: nc });
        }
      }
    }
    return moves;
  }

  // Sliding pieces: bishop, rook, queen
  function generateSlidingMoves(board, r, c, side, directions) {
    const moves = [];

    for (const [dr, dc] of directions) {
      let nr = r + dr;
      let nc = c + dc;
      while (inBounds(nr, nc)) {
        const target = board[nr][nc];
        if (isEmpty(target)) {
          moves.push({ toRow: nr, toCol: nc });
        } else {
          if (
            (side === "W" && isBlackPiece(target)) ||
            (side === "B" && isWhitePiece(target))
          ) {
            moves.push({ toRow: nr, toCol: nc });
          }
          break;
        }
        nr += dr;
        nc += dc;
      }
    }

    return moves;
  }

  // King moves (no castling)
  function generateKingMoves(board, r, c, side) {
    const moves = [];
    for (let dr = -1; dr <= 1; dr++) {
      for (let dc = -1; dc <= 1; dc++) {
        if (dr === 0 && dc === 0) continue;
        const nr = r + dr;
        const nc = c + dc;
        if (inBounds(nr, nc)) {
          const target = board[nr][nc];
          if (
            isEmpty(target) ||
            (side === "W" && isBlackPiece(target)) ||
            (side === "B" && isWhitePiece(target))
          ) {
            moves.push({ toRow: nr, toCol: nc });
          }
        }
      }
    }
    return moves;
  }

  return (
    <div className="board-container">
      <div className="board">
        {board.map((row, rowIndex) =>
          row.map((piece, colIndex) => {
            const visualRow = 7 - rowIndex; // white at bottom
            const isDarkSquare = (visualRow + colIndex) % 2 === 1;
            const canDrag = piece !== "EMPTY" && canDragPiece(piece);

            return (
              <div
                key={`${rowIndex}-${colIndex}`}
                className={`square ${isDarkSquare ? "dark" : "light"}`}
                onDragOver={handleDragOver}
                onDrop={(e) => handleDrop(e, rowIndex, colIndex)}
              >
                {piece !== "EMPTY" && (
                  <div
                    className="piece-container"
                    style={{
                      width: "100%",
                      height: "100%",
                      display: "flex",
                      justifyContent: "center",
                      alignItems: "center"
                    }}
                  >
                    <span
                      className="piece"
                      draggable={canDrag}
                      onDragStart={(e) => {
                        handleDragStart(e, rowIndex, colIndex);
                        e.stopPropagation();
                      }}
                      style={{
                        cursor: canDrag ? "grab" : "default",
                        userSelect: "none",
                        touchAction: "none"
                      }}
                    >
                      {pieceSymbols[piece]}
                    </span>
                  </div>
                )}
              </div>
            );
          })
        )}
      </div>
    </div>
  );
}
