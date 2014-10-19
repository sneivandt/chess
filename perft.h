#ifndef PERFT_H
#define PERFT_H

#include <iostream>
#include <fstream>

#include "board.h"
#include "move.h"
#include "movelist.h"
#include "movegen.h"
#include "makemove.h"

class Perft
{
    private:

        // Numebr of nodes
        long long nodes;

        // Search depth
        int depth;

        // Test a position
        void testPosition(const std::string, Board&);

        // Inner recursive function
        void testPositionInner(const int, Board);

    public:

        // Test the positions
        void test();
};

#endif
