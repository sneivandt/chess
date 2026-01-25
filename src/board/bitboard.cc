#include "bitboard.h"

#include <bit>
#include <iostream>

int board::bitboard::popBit(uint64_t& bb)
{
    int sq = std::countr_zero(bb);
    bb &= (bb - 1);
    return sq;
}

int board::bitboard::countBits(uint64_t bb)
{
    return std::popcount(bb);
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
