#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "board.h"
#include "move.h"
#include "movelist.h"
#include <cmath>
#include <random>

namespace movegen
{

// Sliding pieces
const int SLIDERS[6] = { WQ, WR, WB , BQ, BR, BB };

// Non-sliding pieces
const int NON_SLIDERS[4] = { WN, WK, BN, BK };

// Pawn ranks for promotion
const int PAWN_RANK[2] = { RANK_7, RANK_2 };

// Promotion pieces
const int PROMOTION_PIECES[2][4] = {{ WQ, WR, WB, WN }, { BQ, BR, BB, BN }};

// MVVLVA victum scores
const int VICTUM_SCORES[13] = { 0, 100, 200, 300, 400, 500, 600, 100, 200, 300, 400, 500, 600 };

// Most valuable victum least valuable attacker scores
extern int MVVLVA_SCORES[13][13];

// Init
void INIT();

// Add a quiet move
inline void addQuietMove(const int, MoveList&, Board&);

// Add a capture move
inline void addCaptureMove(const int, MoveList&, Board&);

// Add an En passant move
inline void addEnPasMove(const int, MoveList&);

// Add pawn move
inline void addPawnMove(const int, const int, const int, const int, MoveList&, Board&);

// Generate all moves
MoveList generateAll(Board&, const bool);

}

#endif
