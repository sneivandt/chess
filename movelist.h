#ifndef MOVELIST_H
#define MOVELIST_H

#include "move.h"
#include <iomanip>
#include <iostream>

class MoveList
{
    private:

        // Moves
        Move moves[256];

        // Number of moves
        int count = 0;

    public:

        // Add a move
        void addMove(const Move);

        // Print the move list
        void print() const;

        // Get the count
        int getCount() const { return count; };

        // Get the moves
        Move* getMoves() { return moves; };
};

#endif
