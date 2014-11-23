#include "bitboard.h"

int bitboard::popBit(uint64_t &bb)
{
    uint64_t b = bb ^ (bb - 1);
    unsigned int fold = (unsigned) ((b & 0xffffffff) ^ (b >> 32));
    bb &= (bb - 1);
    return BITTABLE[(fold * 0x783a9b23) >> 26];
}

int bitboard::countBits(uint64_t bb)
{
    int r;
    for(r = 0; bb; r++, bb &= bb - 1);
    return r;
}

void bitboard::clearBit(uint64_t &bb, int sq)
{
    bb &= ~(1ULL << sq);
}

void bitboard::setBit(uint64_t &bb, int sq)
{
    bb |= (1ULL << sq);
}

void bitboard::print(uint64_t bb)
{
    for(int i = 0; i < 64; i++) {
        if(i % 8 == 0) {
            std::cout << std::endl;
        }
        if(bb & 1ULL) {
            std::cout << "X";
        }
        else {
            std::cout << "-";
        }
        bb = bb >> 1;
    }
    std::cout << std::endl;
}
