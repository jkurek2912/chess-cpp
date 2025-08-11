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
  ["WR", "WN", "WB", "WQ", "WK", "WB", "WN", "WR"],
  ["WP", "WP", "WP", "WP", "WP", "WP", "WP", "WP"],
  ["EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY"],
  ["EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY"],
  ["EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY"],
  ["EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY", "EMPTY"],
  ["BP", "BP", "BP", "BP", "BP", "BP", "BP", "BP"],
  ["BR", "BN", "BB", "BQ", "BK", "BB", "BN", "BR"]
];

export default function Board() {
  const [board, setBoard] = useState(initialBoard);

  return (
    <div className="board">
      {board
      .slice()
      .reverse()
      .map((row, r) =>
        row.map((piece, c) => {
          const isDarkSquare = (r + c) % 2 === 1;
          return (
            <div
              key={`${r}-${c}`}
              className={`square ${isDarkSquare ? "dark" : "light"}`}
            >
              <span className="piece">{pieceSymbols[piece]}</span>
            </div>
          );
        })
      )}
    </div>
  );
}
