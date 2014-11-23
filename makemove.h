#ifndef MAKEMOVE_H
#define MAKEMOVE_H

#include "bitboard.h"
#include "board.h"
#include "move.h"
#include "undo.h"

namespace makemove
{

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

}

#endif
