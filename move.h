#ifndef MOVE_H
#define MOVE_H

#include "board.h"
#include <iostream>

// Construct a move value
#define MOVE(from, to, capture, promoted, flag) (((from) | ((to) << 7)) | ((capture) << 14) | ((promoted) << 20) | (flag))

// Get the from square
#define FROMSQ(m) (m & 0x7f)

// Get the to square
#define TOSQ(m) ((m >> 7) & 0x7f)

// Get the captured piece
#define CAPTURED(m) ((m >> 14) & 0xf)

// Get the promoted piece
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

class Move
{
    private:

        // Value
        int value;

        // Score
        int score;

    public:

        // Default Constructor
        Move(): value(0), score(0) {};

        // Create a move with a given value and score
        Move(const int value, const int score): value(value), score(score) {};

        // Less than operator
        bool operator< (Move &other) const { return score < other.getScore(); };

        // Get string representation of the move
        std::string getString() const;

        // Add to the score
        void addScore(const int value) { score += value; };

        // Get the value
        int getValue() const { return value; };

        // Get the score
        int getScore() const { return score; };
};

#endif
