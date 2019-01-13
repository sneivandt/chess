#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "board/board.h"
#include "search/movelist.h"

namespace search { namespace movegen {

// Sliding pieces
const int SLIDERS[6] = {board::WQ, board::WR, board::WB, board::BQ, board::BR, board::BB};

// Non-sliding pieces
const int NON_SLIDERS[4] = {board::WN, board::WK, board::BN, board::BK};

// Pawn ranks for promotion
const int PAWN_RANK[2] = {board::RANK_7, board::RANK_2};

// Promotion pieces
const int PROMOTION_PIECES[2][4] = {{board::WQ, board::WR, board::WB, board::WN},
                                    {board::BQ, board::BR, board::BB, board::BN}};

// MVVLVA victum scores
const int VICTUM_SCORES[13] = {0, 100, 200, 300, 400, 500, 600, 100, 200, 300, 400, 500, 600};

// Most valuable victum least valuable attacker scores
extern int MVVLVA_SCORES[13][13];

// Init
void INIT();

// Add a quiet move
inline void addQuietMove(const int, MoveList&, board::Board&);

// Add a capture move
inline void addCaptureMove(const int, MoveList&, board::Board&);

// Add an En passant move
inline void addEnPasMove(const int, MoveList&);

// Add pawn move
inline void addPawnMove(const int, const int, const int, const int, MoveList&, board::Board&);

// Generate all moves
MoveList generateAll(board::Board&, const bool);

}} // namespace search::movegen

#endif
