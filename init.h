#ifndef INIT_H
#define INIT_H

#include "board.h"
#include "movegen.h"
#include <cmath>
#include <random>

namespace init
{

// MVVLVA victum scores
const int VICTUM_SCORES[13] = { 0, 100, 200, 300, 400, 500, 600, 100, 200, 300, 400, 500, 600 };

// Generate Zobrist hashing keys
void zobristKeys();

// Generate most valuable victum least valuable attacker score table
void mvvlva();

// Generate bitmasks
void bitmasks();

// Initialize all
void all();

}

#endif
