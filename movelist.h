#ifndef MOVELIST_H
#define MOVELIST_H

#include "move.h"
#include <iomanip>
#include <iostream>

// Maximum moves per position
const int MAX_POSITION_MOVES = 256;

class MoveList
{
    private:

        // Moves
        Move moves[MAX_POSITION_MOVES];

        // Number of moves
        int count;

    public:

        // Constructor
        MoveList() { count = 0; };

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
