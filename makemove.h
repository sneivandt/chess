#ifndef MAKEMOVE_H
#define MAKEMOVE_H

#include "board.h"
#include "move.h"

// Remove a piece from the board
void clearPiece(const int, Board&);

// Add a piece to the board
void addPiece(const int, const int, Board&);

// Move a piece
void movePiece(const int, const int, Board&);

// Make a move
bool makeMove(Move&, Board&);

// Take back a move
void takeMove(Board&);

#endif
