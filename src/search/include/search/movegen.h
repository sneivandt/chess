#pragma once

#include "board/board.h"
#include "search/movelist.h"

#include <array>

namespace search {

struct SearchState;

namespace movegen {

// Sliding pieces
const std::array<int, 6> SLIDERS = {toInt(board::Piece::WQ), toInt(board::Piece::WR), toInt(board::Piece::WB),
                                    toInt(board::Piece::BQ), toInt(board::Piece::BR), toInt(board::Piece::BB)};

// Non-sliding pieces
const std::array<int, 4> NON_SLIDERS = {toInt(board::Piece::WN), toInt(board::Piece::WK), toInt(board::Piece::BN),
                                        toInt(board::Piece::BK)};

// Pawn ranks for promotion
const std::array<int, 2> PAWN_RANK = {toInt(board::Rank::RANK_7), toInt(board::Rank::RANK_2)};

// Pawn starting ranks for double moves
const std::array<int, 2> PAWN_START_RANK = {toInt(board::Rank::RANK_2), toInt(board::Rank::RANK_7)};

// Promotion pieces
const std::array<std::array<int, 4>, 2> PROMOTION_PIECES = {
    {{toInt(board::Piece::WQ), toInt(board::Piece::WR), toInt(board::Piece::WB), toInt(board::Piece::WN)},
     {toInt(board::Piece::BQ), toInt(board::Piece::BR), toInt(board::Piece::BB), toInt(board::Piece::BN)}}};

// MVVLVA victum scores
const std::array<int, 13> VICTUM_SCORES = {0, 100, 200, 300, 400, 500, 600, 100, 200, 300, 400, 500, 600};

// Most valuable victum least valuable attacker scores
extern std::array<std::array<int, 13>, 13> MVVLVA_SCORES;

// Init
void INIT();

// Add a quiet move
inline void addQuietMove(const int, MoveList&, board::Board&, const SearchState*);

// Add a capture move
inline void addCaptureMove(const int, MoveList&, board::Board&);

// Add an En passant move
inline void addEnPasMove(const int, MoveList&);

// Add pawn move
inline void addPawnMove(const int, const int, const int, const int, MoveList&, board::Board&, const SearchState*);

// Generate pawn moves
void generatePawnMoves(board::Board&, MoveList&, const bool, const SearchState*);

// Generate sliding piece moves
void generateSliderMoves(board::Board&, MoveList&, const bool, const SearchState*);

// Generate non-sliding piece moves
void generateNonSliderMoves(board::Board&, MoveList&, const bool, const SearchState*);

// Generate castling moves
void generateCastlingMoves(board::Board&, MoveList&, const SearchState*);

// Generate all moves
MoveList generateAll(board::Board&, const bool, const SearchState* = nullptr);

} // namespace movegen
} // namespace search
