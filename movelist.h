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

        // Print the move list
        void print() const;

        // Moves
        std::vector<Move> getMoves();
        void addMove(const Move);
};

inline std::vector<Move> MoveList::getMoves()
{
    return moves;
}

inline void MoveList::addMove(const Move move)
{
    moves.push_back(move);
}

#endif
