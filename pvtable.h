#ifndef PVTABLE_H
#define PVTABLE_H

#include "board.h"
#include "makemove.h"
#include "move.h"
#include <cstdint>
#include <unordered_map>
#include <vector>

class PVTable
{
    private:

        // Principal variation hash table
        std::unordered_map <uint64_t, Move>map;

    public:

        // Constructor
        PVTable() {};

        // Add a move to the map
        void addMove(Board&, Move&);

        // Get a move from the map
        Move getMove(Board&) const;

        // Get the principal variation
        std::vector<Move> getPV(Board&);
};

#endif
