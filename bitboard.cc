#include "bitboard.h"

int popBit(uint64_t &bb)
{
    uint64_t b = bb ^ (bb - 1);
    unsigned int fold = (unsigned) ((b & 0xffffffff) ^ (b >> 32));
    bb &= (bb - 1);
    return BITTABLE[(fold * 0x783a9b23) >> 26];
}

int countBits(uint64_t bb)
{
    int r;
    for(r = 0; bb; r++, bb &= bb - 1);
    return r;
}

void clearBit(uint64_t &bb, int sq)
{
    bb &= ~(1ULL << sq);
}

void setBit(uint64_t &bb, int sq)
{
    bb |= (1ULL << sq);
}
