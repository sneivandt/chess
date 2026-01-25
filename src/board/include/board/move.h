#ifndef MOVE_H
#define MOVE_H

#include <string>

namespace board {

class Move
{
  private:
    // Value
    int value;

    // Score
    int score;

  public:
    Move() : value(0), score(0) {};
    Move(const int v, const int s) : value(v), score(s) {};

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
    bool operator<(const Move&) const;

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

/*
 * Move integer layout:
 *
 *  0 -  6 : From square (7 bits)
 *  7 - 13 : To square (7 bits)
 * 14 - 17 : Captured piece (4 bits)
 *      18 : En Passant flag (1 bit)
 *      19 : Pawn Start flag (1 bit)
 * 20 - 23 : Promoted piece (4 bits)
 *      24 : Castle flag (1 bit)
 *
 * Captured Piece limits: 0-15 (4 bits)
 * Promoted Piece limits: 0-15 (4 bits)
 */

inline int Move::MOVE(const int from, const int to, const int capture, const int promoted, const int flag)
{
    return (from & 0x7f) | ((to & 0x7f) << 7) | ((capture & 0xf) << 14) | ((promoted & 0xf) << 20) | flag;
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

inline bool Move::operator<(const Move& rhs) const
{
    return score < rhs.getScore();
}

inline int Move::getScore() const
{
    return score;
}

inline void Move::addScore(const int s)
{
    score += s;
}

inline int Move::getValue() const
{
    return value;
}

} // namespace board

#endif
