#include "bitboard.h"

#include "board/board.h"

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
    print(bb, std::cout);
}

void board::bitboard::print(uint64_t bb, std::ostream& output)
{
    for (int i = 0; i < BOARD64_SQUARE_NUM; i++) {
        if (i % FILE_NUM == 0) {
            output << std::endl;
        }
        if (bb & 1ULL) {
            output << "X";
        }
        else {
            output << "-";
        }
        bb = bb >> 1;
    }
    output << std::endl;
}
