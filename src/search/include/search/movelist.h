#ifndef MOVELIST_H
#define MOVELIST_H

#include "board/move.h"

#include <iomanip>
#include <iostream>
#include <vector>

namespace search {

class MoveList
{
  private:
    // Moves
    std::vector<board::Move> moves;

  public:
    // Print the move list
    void print() const;

    // Moves
    std::vector<board::Move> getMoves();
    void addMove(const board::Move);
};

inline std::vector<board::Move> MoveList::getMoves()
{
    return moves;
}

inline void MoveList::addMove(const board::Move move)
{
    moves.push_back(move);
}

} // namespace search

#endif
