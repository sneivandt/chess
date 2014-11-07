#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "board.h"
#include "move.h"
#include "movelist.h"

// Sliding pieces
const int SLIDERS[6] = { WQ, WR, WB , BQ, BR, BB };

// Non-sliding pieces
const int NON_SLIDERS[4] = { WN, WK, BN, BK };

// Pawn ranks for promotion
const int PAWN_RANK[2] = { RANK_7, RANK_2 };

// Promotion pieces
const int PROMOTION_PIECES[2][4] = {{ WQ, WR, WB, WN }, { BQ, BR, BB, BN }};

// Add a quiet move
inline void addQuietMove(const int, MoveList&);

// Add a capture move
inline void addCaptureMove(const int, MoveList&);

// Add an En passant move
inline void addEnPasMove(const int, MoveList&);

// Add pawn move
inline void addPawnMove(const int, const int, const int, MoveList&);

// Add pawn capture move
inline void addPawnMove(const int, const int, const int, const int, MoveList&);

// Generate all moves
MoveList generateAllMoves(Board&);

#endif
