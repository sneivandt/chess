#ifndef PVTABLE_H
#define PVTABLE_H

#include "board/move.h"

#include <cstdint>
#include <unordered_map>
#include <vector>

namespace board {
class Board;
}

namespace search {

class PVTable
{
  private:
    // Principal variation hash table
    std::unordered_map<uint64_t, board::Move> map;

  public:
    // Default Constructor
    PVTable() {};

    // Add a move to the hash table
    void addMove(board::Board&, board::Move&);

    // Get a move from the hash table
    board::Move getMove(board::Board&) const;

    // Get the principal variation
    std::vector<board::Move> getPV(board::Board&);
};

} // namespace search

#endif
