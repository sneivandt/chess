#ifndef MOVE_H
#define MOVE_H

#include <iostream>

// Construct a move
#define MOVE(from, to, capture, promoted, flag) (((from) | ((to) << 7)) | ((capture) << 14) | ((promoted) << 20) | (flag))

// Get from square
#define FROMSQ(m) (m & 0x7f)

// Get to square
#define TOSQ(m) ((m >> 7) & 0x7f)

// Get captured piece
#define CAPTURED(m) ((m >> 14) & 0xf)

// Get promoted piece
#define PROMOTED(m) ((m >> 20) & 0xf)

// En passant flag
const int MFLAGEP = 0x40000;

// Pawn start flag
const int MFLAGPS = 0x80000;

// Castle flag
const int MFLAGCA = 0x1000000;

// Capture flag
const int MFLAGCAP = 0x7c000;

// Promotion flag
const int MFLAGPROM = 0xf00000;

// TODO: remove this duplication. This array is defined in board.h but
// including board.h directly in move.h causes erros due to a cycle of
// header includes ???
const int SQ64_2[] = {
    99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
    99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
    99,  0,  1,  2,  3,  4,  5,  6,  7, 99,
    99,  8,  9, 10, 11, 12, 13, 14, 15, 99,
    99, 16, 17, 18, 19, 20, 21, 22, 23, 99,
    99, 24, 25, 26, 27, 28, 29, 30, 31, 99,
    99, 32, 33, 34, 35, 36, 37, 38, 39, 99,
    99, 40, 41, 42, 43, 44, 45, 46, 47, 99,
    99, 48, 49, 50, 51, 52, 53, 54, 55, 99,
    99, 56, 57, 58, 59, 60, 61, 62, 63, 99,
    99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
    99, 99, 99, 99, 99, 99, 99, 99, 99, 99
};

// Get the algebraic name of a square
std::string squareName(const int);

class Move
{
    private:

        // Value
        int value;

        // Score
        int score;

    public:

        // Default Constructor
        Move() {};

        // Constructor
        Move(int value, int score): value(value), score(score) {};

        // Print a move
        void print() const;

        // Get value
        int getValue() const { return value; };
};

#endif
