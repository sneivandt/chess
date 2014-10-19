#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "board.h"
#include "move.h"
#include "movelist.h"

// Sliding pieces
const int SLIDERS[6] = { WQ, WR, WB , BQ, BR, BB };

// Non-sliding pieces
const int NON_SLIDERS[4] = { WN, WK, BN, BK };

// Add a quiet move
inline void addQuietMove(Board&, const int, MoveList&);

// Add a capture move
inline void addCaptureMove(Board&, const int, MoveList&);

// Add an En passant move
inline void addEnPasMove(Board&, const int, MoveList&);

// Add white pawn move
inline void addWhitePawnMove(Board&, const int, const int, MoveList&);

// Add white pawn capture move
inline void addWhitePawnCaptureMove(Board&, const int, const int, const int, MoveList&);

// Add black pawn move
inline void addBlackPawnMove(Board&, const int, const int, MoveList&);

// Add black pawn capture move
inline void addBlackPawnCaptureMove(Board&, const int, const int, const int, MoveList&);

// Generate all moves
MoveList generateAllMoves(Board&);

#endif
