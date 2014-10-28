#include "init.h"

// Zobrist piece hash keys
uint64_t PIECE_KEYS[13][120];

// Zobrist castle hash keys
uint64_t CASTLE_KEYS[16];

// Zobrist side hash key
uint64_t SIDE_KEY;

void initZobristKeys()
{
    std::random_device rd;
    std::mt19937_64 e2(rd());
    std::uniform_int_distribution<long long int> dist(std::llround(std::pow(2,61)), std::llround(std::pow(2,62)));
    for(int i = 0; i < 13; i++) {
        for(int j = 0; j < 120; j++) {
            PIECE_KEYS[i][j] = dist(e2);
        }
    }
    for(int i = 0; i < 16; i++) {
        CASTLE_KEYS[i] = dist(e2);
    }
    SIDE_KEY = dist(e2);
}

void init()
{
    initZobristKeys();
}
