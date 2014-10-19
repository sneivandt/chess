#ifndef PERFT_H
#define PERFT_H

#include "board.h"
#include "move.h"
#include "movelist.h"
#include "movegen.h"
#include "makemove.h"
#include <cassert>
#include <string>
#include <iostream>
#include <fstream>

const std::string TEST_SPECS = "perft.in";

class Perft
{
    private:

        // Numebr of nodes
        long long nodes;

        // Total number of positions evaluated
        long long totalPositions;

        // Search depth
        int depth;

        // Test a position
        void testPosition(const std::string, Board&);

        // Inner recursive function
        void testPositionInner(const int, Board);

    public:

        // Test the positions from perft.in
        void test();
};

#endif
