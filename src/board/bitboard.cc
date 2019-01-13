#include "bitboard.h"

#include <iostream>

int board::bitboard::popBit(uint64_t& bb)
{
    uint64_t b = bb ^ (bb - 1);
    auto fold = static_cast<unsigned>((b & 0xffffffff) ^ (b >> 32));
    bb &= (bb - 1);
    return BITTABLE[(fold * 0x783a9b23) >> 26];
}

int board::bitboard::countBits(uint64_t bb)
{
    int r;
    for (r = 0; bb; r++, bb &= bb - 1) {
        ;
    }
    return r;
}

void board::bitboard::clearBit(uint64_t& bb, int sq)
{
    bb &= ~(1ULL << sq);
}

void board::bitboard::setBit(uint64_t& bb, int sq)
{
    bb |= (1ULL << sq);
}

void board::bitboard::print(uint64_t bb)
{
    for (int i = 0; i < 64; i++) {
        if (i % 8 == 0) {
            std::cout << std::endl;
        }
        if (bb & 1ULL) {
            std::cout << "X";
        }
        else {
            std::cout << "-";
        }
        bb = bb >> 1;
    }
    std::cout << std::endl;
}
