#ifndef PERFT_H
#define PERFT_H

#include "board/board.h"
#include "board/makemove.h"
#include "board/move.h"
#include "search/movegen.h"
#include "search/movelist.h"
#include "utils/utils.h"

#include "gtest/gtest.h"

#include <fstream>
#include <iostream>

namespace test {
namespace perft {

// Test all the positions
void test();

// Test a position
void testPosition(const std::string, board::Board&);

// Count the moves in position
int64_t countMoves(const int, board::Board&);

} // namespace perft
} // namespace test

#endif
