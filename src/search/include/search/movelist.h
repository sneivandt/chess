#ifndef MOVELIST_H
#define MOVELIST_H

#include "board/move.h"

#include <algorithm>
#include <vector>

namespace search {

class MoveList
{
  private:
    // Moves
    std::vector<board::Move> moves;

  public:
    // Move semantics
    MoveList() = default;
    MoveList(const MoveList&) = default;
    MoveList& operator=(const MoveList&) = default;
    MoveList(MoveList&&) noexcept = default;
    MoveList& operator=(MoveList&&) noexcept = default;

    // Print the move list
    void print() const;

    // Moves - return const reference instead of copy
    const std::vector<board::Move>& getMoves() const noexcept;
    void addMove(const board::Move);
};

inline const std::vector<board::Move>& MoveList::getMoves() const noexcept
{
    return moves;
}

inline void MoveList::addMove(const board::Move move)
{
    moves.push_back(move);
}

} // namespace search

#endif
