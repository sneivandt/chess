#ifndef PERFT_H
#define PERFT_H

#include <cstdint>
#include <string>

namespace board {
class Board;
}

namespace test { namespace perft {

// Test all the positions
void test();

// Test a position
void testPosition(const std::string&, board::Board&);

// Count the moves in position
int64_t countMoves(const int, board::Board&);

}} // namespace test::perft

#endif
