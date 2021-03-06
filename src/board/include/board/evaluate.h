#ifndef EVALUATE_H
#define EVALUATE_H

namespace board {
class Board;
}

namespace board { namespace evaluate {

// Endgame material threshold
const int ENDGAME_MATERIAL = 1100;

// Isolated pawn score
const int PAWN_ISOLATED = -10;

// Passed pawn score
const int PASSED_PAWN[8] = {0, 5, 10, 20, 35, 60, 100, 200};

// Piece on open file score
const int PIECE_OPEN_FILE = 5;

// Rook open file score
const int ROOK_OPEN_FILE = 10;

// Rook semi-open file score
const int ROOK_SEMI_OPEN_FILE = 5;

// Queen open file score
const int QUEEN_OPEN_FILE = 5;

// Queen semi-open file score
const int QUEEN_SEMI_OPEN_FILE = 3;

// Bishop pair score
const int BISHOP_PAIR = 30;

// Piece square tables
const int PIECE_SQUARE_TABLE[][64] = {
    {},
    {// Pawn
     0,  0,  0,  0,  0,  0,  0,  0,  10, 10, 0,  -10, -10, 0,  10, 10, 5, 0, 0,  5,  5,  0,
     0,  5,  0,  0,  10, 20, 20, 10, 0,  0,  5,  5,   5,   10, 10, 5,  5, 5, 10, 10, 10, 20,
     20, 10, 10, 10, 20, 20, 20, 30, 30, 20, 20, 20,  0,   0,  0,  0,  0, 0, 0,  0},
    {// Knight
     0,  -10, 0,  0, 0,  0,  -10, 0,  0,  0, 0, 5,  5,  0,  0,  0,  0,  0, 10, 10, 10, 10,
     0,  0,   0,  0, 10, 20, 20,  10, 5,  0, 5, 10, 15, 20, 20, 15, 10, 5, 5,  10, 10, 20,
     20, 10,  10, 5, 0,  0,  5,   10, 10, 5, 0, 0,  0,  0,  0,  0,  0,  0, 0,  0},
    {// Bishop
     0,  0,  -10, 0,  0,  -10, 0,  0,  0,  0, 0, 10, 10, 0,  0,  0,  0,  0, 10, 15, 15, 10,
     0,  0,  0,   10, 15, 20,  20, 15, 10, 0, 0, 10, 15, 20, 20, 15, 10, 0, 0,  0,  10, 15,
     15, 10, 0,   0,  0,  0,   0,  10, 10, 0, 0, 0,  0,  0,  0,  0,  0,  0, 0,  0},
    {// Rook
     0, 0, 5, 10, 10, 5, 0, 0, 0, 0, 5, 10, 10, 5, 0, 0, 0,  0,  5,  10, 10, 5,  0,  0,  0, 0, 5, 10, 10, 5, 0, 0,
     0, 0, 5, 10, 10, 5, 0, 0, 0, 0, 5, 10, 10, 5, 0, 0, 25, 25, 25, 25, 25, 25, 25, 25, 0, 0, 5, 10, 10, 5, 0, 0},
    {},
    {}};

// Piece square table king opening
const int PIECE_SQUARE_TABLE_KING_OPENING[64] = {
    0,   5,   5,   -10, -10, 0,   10,  5,   -10, -10, -10, -10, -10, -10, -10, -10, -30, -30, -30, -30, -30, -30,
    -30, -30, -70, -70, -70, -70, -70, -70, -70, -70, -70, -70, -70, -70, -70, -70, -70, -70, -70, -70, -70, -70,
    -70, -70, -70, -70, -70, -70, -70, -70, -70, -70, -70, -70, -70, -70, -70, -70, -70, -70, -70, -70};

// Piece square table king opening
const int PIECE_SQUARE_TABLE_KING_END[64] = {-50, -10, 0,  0,  0,  0,  -10, -50, -10, 0,   10, 10, 10, 10, 0,   -10,
                                             0,   10,  15, 15, 15, 15, 10,  0,   0,   10,  15, 20, 20, 15, 10,  0,
                                             0,   10,  15, 20, 20, 15, 10,  0,   0,   10,  15, 15, 15, 15, 10,  0,
                                             -10, 0,   10, 10, 10, 10, 0,   -10, -50, -10, 0,  0,  0,  0,  -10, -50};

// Mirror a piece square table index
const int MIRROR_PIECE_SQUARE_TABLE[64] = {56, 57, 58, 59, 60, 61, 62, 63, 48, 49, 50, 51, 52, 53, 54, 55,
                                           40, 41, 42, 43, 44, 45, 46, 47, 32, 33, 34, 35, 36, 37, 38, 39,
                                           24, 25, 26, 27, 28, 29, 30, 31, 16, 17, 18, 19, 20, 21, 22, 23,
                                           8,  9,  10, 11, 12, 13, 14, 15, 0,  1,  2,  3,  4,  5,  6,  7};

// Material draw
inline bool materialDraw(Board&);

// Is endgame
inline bool isEndGame(Board&, const int);

// Evaluate a board
int score(Board&);

}} // namespace board::evaluate

#endif
