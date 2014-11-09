#ifndef INIT_H
#define INIT_H

#include "board.h"
#include <cmath>
#include <random>

// Generate Zobrist hashing keys
void initZobristHeys();

// Generate most valuable victum least valuable attacker score table
void initMVVLVA();

// Initialize all
void init();

#endif
