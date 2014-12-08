#ifndef MOVE_H
#define MOVE_H

#include "board.h"
#include <iostream>

class Move
{
    private:

        // Value
        int value;

        // Score
        int score;

    public:

        Move(): value(0), score(0) {};
        Move(const int value, const int score): value(value), score(score) {};

        // From square
        static int FROMSQ(const int);

        // Get the to square
        static int TOSQ(const int);

        // Get the captured piece
        static int CAPTURED(const int);

        // Get the promoted piece
        static int PROMOTED(const int);

        // En passant flag
        const static int MFLAGEP = 0x40000;

        // Pawn start flag
        const static int MFLAGPS = 0x80000;

        // Castle flag
        const static int MFLAGCA = 0x1000000;

        // Capture flag
        const static int MFLAGCAP = 0x7c000;

        // Promotion flag
        const static int MFLAGPROM = 0xf00000;

        // Less than operator
        bool operator<(Move&) const;

        // Get string representation of the move
        std::string getString() const;

        // Score
        int getScore() const;
        void addScore(const int);

        // Value
        int getValue() const;

        // Construct a move value
        static int MOVE(const int, const int, const int, const int, const int);
};

inline int Move::MOVE(const int from, const int to, const int capture, const int promoted, const int flag)
{
    return from | (to << 7) | (capture << 14) | (promoted << 20) | flag;
}

inline int Move::FROMSQ(const int move)
{
    return move & 0x7f;
}

inline int Move::TOSQ(const int move)
{
    return (move >> 7) & 0x7f;
}

inline int Move::CAPTURED(const int move)
{
    return (move >> 14) & 0xf;
}

inline int Move::PROMOTED(const int move)
{
    return (move >> 20) & 0xf;
}

inline bool Move::operator<(Move &other) const
{
    return score < other.getScore();
}

inline int Move::getScore() const
{
    return score;
}

inline void Move::addScore(const int value)
{
    score += value;
}

inline int Move::getValue() const
{
    return value;
}

#endif
