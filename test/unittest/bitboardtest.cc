#include "../../src/board/bitboard.h"

#include "unittest.h"

#include <cstdint>

using namespace test;

class BitboardTest : public test::UnitTest
{
};

TEST_F(BitboardTest, SetAndClearBit)
{
    uint64_t bb = 0ULL;

    board::bitboard::setBit(bb, 0);
    board::bitboard::setBit(bb, 7);
    board::bitboard::setBit(bb, 63);

    EXPECT_EQ(bb, (1ULL << 0) | (1ULL << 7) | (1ULL << 63));

    board::bitboard::clearBit(bb, 7);

    EXPECT_EQ(bb, (1ULL << 0) | (1ULL << 63));
}

TEST_F(BitboardTest, CountBits)
{
    EXPECT_EQ(board::bitboard::countBits(0ULL), 0);
    EXPECT_EQ(board::bitboard::countBits(1ULL), 1);
    EXPECT_EQ(board::bitboard::countBits((1ULL << 3) | (1ULL << 11) | (1ULL << 42)), 3);
}

TEST_F(BitboardTest, PopBitReturnsAndClearsLeastSignificantBit)
{
    uint64_t bb = (1ULL << 2) | (1ULL << 5) | (1ULL << 9);

    EXPECT_EQ(board::bitboard::popBit(bb), 2);
    EXPECT_EQ(bb, (1ULL << 5) | (1ULL << 9));

    EXPECT_EQ(board::bitboard::popBit(bb), 5);
    EXPECT_EQ(bb, (1ULL << 9));

    EXPECT_EQ(board::bitboard::popBit(bb), 9);
    EXPECT_EQ(bb, 0ULL);
}

TEST_F(BitboardTest, PrintRendersBitsFromLeastSignificantSquare)
{
    uint64_t bb = 0ULL;
    board::bitboard::setBit(bb, 0);
    board::bitboard::setBit(bb, 9);
    board::bitboard::setBit(bb, 63);

    board::bitboard::print(bb);

    EXPECT_EQ(buffer.str(), "\nX-------\n-X------\n--------\n--------\n--------\n--------\n--------\n-------X\n");
}
