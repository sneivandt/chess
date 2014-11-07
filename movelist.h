#ifndef MOVELIST_H
#define MOVELIST_H

#include "move.h"
#include <iomanip>
#include <iostream>
#include <vector>

class MoveList
{
    private:

        // Moves
        std::vector<Move> moves;

    public:

        // Add a move
        void addMove(const Move);

        // Print the move list
        void print() const;

        // Get the moves
        std::vector<Move> getMoves() { return moves; };
};

#endif
