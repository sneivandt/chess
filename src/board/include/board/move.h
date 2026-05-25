#pragma once

#include <string>

namespace board {

class MoveValue
{
  private:
    int encoded;

  public:
    constexpr MoveValue() noexcept : encoded(0) {};
    constexpr explicit MoveValue(const int value) noexcept : encoded(value) {};

    constexpr int raw() const noexcept
    {
        return encoded;
    }

    constexpr operator int() const noexcept
    {
        return encoded;
    }
};

constexpr bool operator==(const MoveValue lhs, const MoveValue rhs) noexcept
{
    return lhs.raw() == rhs.raw();
}

constexpr bool operator!=(const MoveValue lhs, const MoveValue rhs) noexcept
{
    return !(lhs == rhs);
}

class Move
{
  private:
    // Value
    MoveValue value;

    // Score
    int score;

  public:
    Move() : value(0), score(0) {};
    Move(const MoveValue v, const int s) : value(v), score(s) {};
    Move(const int v, const int s) : Move(MoveValue(v), s) {};

    // Copy semantics (explicitly defaulted)
    Move(const Move&) = default;
    Move& operator=(const Move&) = default;

    // Move semantics
    Move(Move&&) noexcept = default;
    Move& operator=(Move&&) noexcept = default;

    // Bit masks and shifts for move encoding
    static constexpr int FROM_MASK = 0x7f;
    static constexpr int TO_MASK = 0x7f;
    static constexpr int TO_SHIFT = 7;
    static constexpr int CAPTURE_MASK = 0xf;
    static constexpr int CAPTURE_SHIFT = 14;
    static constexpr int PROMOTED_MASK = 0xf;
    static constexpr int PROMOTED_SHIFT = 20;

    // From square
    static int FROMSQ(const MoveValue) noexcept;
    static int FROMSQ(const int) noexcept;

    // Get the to square
    static int TOSQ(const MoveValue) noexcept;
    static int TOSQ(const int) noexcept;

    // Get the captured piece
    static int CAPTURED(const MoveValue) noexcept;
    static int CAPTURED(const int) noexcept;

    // Get the promoted piece
    static int PROMOTED(const MoveValue) noexcept;
    static int PROMOTED(const int) noexcept;

    // En passant flag
    static constexpr int MFLAGEP = 0x40000;

    // Pawn start flag
    static constexpr int MFLAGPS = 0x80000;

    // Castle flag
    static constexpr int MFLAGCA = 0x1000000;

    // Capture flag
    static constexpr int MFLAGCAP = 0x7c000;

    // Promotion flag
    static constexpr int MFLAGPROM = 0xf00000;

    // Less than operator
    bool operator<(const Move&) const noexcept;

    // Get string representation of the move
    std::string getString() const;

    // Score
    int getScore() const noexcept;
    void addScore(const int) noexcept;

    // Value
    MoveValue getValue() const noexcept;

    // Construct a move value
    static MoveValue MOVE(const int, const int, const int, const int, const int) noexcept;
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

inline MoveValue Move::MOVE(const int from, const int to, const int capture, const int promoted,
                            const int flag) noexcept
{
    return MoveValue((from & FROM_MASK) | ((to & TO_MASK) << TO_SHIFT) | ((capture & CAPTURE_MASK) << CAPTURE_SHIFT) |
                     ((promoted & PROMOTED_MASK) << PROMOTED_SHIFT) | flag);
}

inline int Move::FROMSQ(const MoveValue move) noexcept
{
    return move.raw() & FROM_MASK;
}

inline int Move::FROMSQ(const int move) noexcept
{
    return FROMSQ(MoveValue(move));
}

inline int Move::TOSQ(const MoveValue move) noexcept
{
    return (move.raw() >> TO_SHIFT) & TO_MASK;
}

inline int Move::TOSQ(const int move) noexcept
{
    return TOSQ(MoveValue(move));
}

inline int Move::CAPTURED(const MoveValue move) noexcept
{
    return (move.raw() >> CAPTURE_SHIFT) & CAPTURE_MASK;
}

inline int Move::CAPTURED(const int move) noexcept
{
    return CAPTURED(MoveValue(move));
}

inline int Move::PROMOTED(const MoveValue move) noexcept
{
    return (move.raw() >> PROMOTED_SHIFT) & PROMOTED_MASK;
}

inline int Move::PROMOTED(const int move) noexcept
{
    return PROMOTED(MoveValue(move));
}

inline bool Move::operator<(const Move& rhs) const noexcept
{
    return score < rhs.getScore();
}

inline int Move::getScore() const noexcept
{
    return score;
}

inline void Move::addScore(const int s) noexcept
{
    score += s;
}

inline MoveValue Move::getValue() const noexcept
{
    return value;
}

} // namespace board
