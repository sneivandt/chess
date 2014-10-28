#ifndef UNDO_H
#define UNDO_H

#include "move.h"
#include <cstdint>

// A previous board
class Undo
{
    private:

        // Side to move
        bool side;

        // Castling permutation
        int castlePerm;

        // En passant square
        int enPas;

        // Fifty move rule count
        int fiftyMove;

        // The move value
        int moveValue;

        // Position key
        uint64_t hashKey;

    public:

        // Default constructor
        Undo() {};

        // Create an undo with the given properties
        Undo(const int side, const int castle, const int enPas, const int fiftyMove, const int moveValue, const uint64_t hash): side(side), castlePerm(castle), enPas(enPas), fiftyMove(fiftyMove), moveValue(moveValue), hashKey(hash) {};

        // Get the move value
        int getMoveValue() const { return moveValue; };

        // Get the castle perm
        int getCastlePerm() const { return castlePerm; };

        // Get the fifty move count
        int getFiftyMove() const { return fiftyMove; };

        // Get the En pasant square
        int getEnPas() const { return enPas; };

        // Get the hash key
        uint64_t getHashKey() const { return hashKey; };
};

#endif
