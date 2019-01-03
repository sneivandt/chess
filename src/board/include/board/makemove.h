#ifndef MAKEMOVE_H
#define MAKEMOVE_H

#include "board/bitboard.h"
#include "board/board.h"
#include "board/move.h"
#include "board/undo.h"

namespace board {
namespace makemove {

// Remove a piece from a board
inline void clearPiece(const int, Board&);

// Add a piece to a board
inline void addPiece(const int, const int, Board&);

// Move a piece
inline void movePiece(const int, const int, Board&);

// Make a move
bool move(Move&, Board&);

// Take back a move
void undo(Board&);

} // namespace makemove
} // namespace board

#endif
