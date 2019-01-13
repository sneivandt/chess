#ifndef MAKEMOVE_H
#define MAKEMOVE_H

namespace board {

class Board;
class Move;

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
