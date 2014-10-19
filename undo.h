#ifndef UNDO_H
#define UNDO_H

#include <cstdint>

#include "move.h"

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

        // The move
        Move move;

        // Position key
        uint64_t hashKey;

    public:

        // Default constructor
        Undo() {};

        // Constructor
        Undo(const int side, const int castle, const int enPas, const int fiftyMove, const Move move, const uint64_t hash): side(side), castlePerm(castle), enPas(enPas), fiftyMove(fiftyMove), move(move), hashKey(hash) {};

        // Get the move
        Move getMove() const { return move; };

        // Get the castle perm
        int getCastlePerm() const { return castlePerm; };

        // Get the fifty move count
        int getFiftyMove() const { return fiftyMove; };

        // Get En pasant square
        int getEnPas() const { return enPas; };
};

#endif
