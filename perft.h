#ifndef PERFT_H
#define PERFT_H

#include "board.h"
#include "makemove.h"
#include "move.h"
#include "movegen.h"
#include "movelist.h"
#include "undo.h"
#include "utils.h"
#include <cassert>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

// Test specs
const std::string TEST_SPECS = "perft.txt";

// Test all the positions
void perftTest();

// Test a position
void testPosition(const std::string, Board&);

// Inner recursive function
long long testPositionInner(const int, Board);

#endif
