import React, { useState } from "react";
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

export default function Board() {
    const [board, setBoard] = useState(initialBoard);
    const [draggedFrom, setDraggedFrom] = useState(null);
    const [sideToMove, setSideToMove] = useState("W");

    function isSideToMove(piece) {
        if (sideToMove === "W") return isWhitePiece(piece);
        else return isBlackPiece(piece);
    }

    function handleDragStart(event, row, col) {
        const piece = board[row][col];
        if (piece !== "EMPTY" && isSideToMove(piece)) {
            event.dataTransfer.setData("text/plain", JSON.stringify({ row, col }));
            setDraggedFrom({ row, col });
            // Required for Firefox
            event.dataTransfer.effectAllowed = "move";
        } else {
            event.preventDefault();
        }
    }

    function handleDragOver(event) {
        event.preventDefault();
        event.dataTransfer.dropEffect = "move";
    }

    function handleDrop(event, toRow, toCol) {
        event.preventDefault();
        if (!draggedFrom) return;

        const { row: fromRow, col: fromCol } = draggedFrom;
        const movingPiece = board[fromRow][fromCol];
        const targetPiece = board[toRow][toCol];

        // Basic validation (can capture opponent's pieces or move to empty square)
        if (targetPiece === "EMPTY" ||
            (isWhitePiece(movingPiece) && isBlackPiece(targetPiece)) ||
            (isBlackPiece(movingPiece) && isWhitePiece(targetPiece))) {

            const newBoard = [...board.map(row => [...row])];
            newBoard[toRow][toCol] = movingPiece;
            newBoard[fromRow][fromCol] = "EMPTY";

            setBoard(newBoard);
            setSideToMove(sideToMove === "W" ? "B" : "W");
        }
        setDraggedFrom(null);
    }

    return (
        <div className="board-container">
            <div className="board">
                {board.map((row, rowIndex) => (
                    row.map((piece, colIndex) => {
                        const visualRow = 7 - rowIndex;
                        const isDarkSquare = (visualRow + colIndex) % 2 === 1;
                        const canDrag = piece !== "EMPTY" && isSideToMove(piece);

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
                ))}
            </div>
        </div>
    );
}