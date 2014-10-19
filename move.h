#ifndef MOVE_H
#define MOVE_H

#include "board.h"
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

// Get the algebraic name of a square
inline std::string squareName(const int);

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
        Move(const int value, const int score): value(value), score(score) {};

        // Print a move
        void print() const;

        // Get value
        int getValue() const { return value; };
};

#endif
