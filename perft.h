#ifndef PERFT_H
#define PERFT_H

#include "board.h"
#include "makemove.h"
#include "move.h"
#include "movegen.h"
#include "movelist.h"
#include "utils.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace perft
{

// Test specs file
const std::string TEST_FILE = "perft.txt";

// Test all the positions
void test();

// Test a position
void testPosition(const std::string, Board&);

// Count the moves in position
long long countMoves(const int, Board&);

}

#endif
