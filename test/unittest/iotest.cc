#include "iotest.h"

#include "io/io.h"
#include "search/movegen.h"
#include "utils/exceptions.h"

#include <gtest/gtest.h>

using namespace test;

TEST_F(IoTest, parseMoveInvalidThrowsException)
{
    // Test that an invalid move throws AceException
    ASSERT_THROW(io::parseMove("invalid", pos), utils::AceException);
}

TEST_F(IoTest, parseMoveInvalidThrowsStdException)
{
    // Test that an invalid move can be caught as std::exception
    try {
        io::parseMove("z9z9", pos);
        FAIL() << "Expected AceException to be thrown";
    }
    catch (const std::exception& e) {
        ASSERT_STREQ(e.what(), "Invalid move");
    }
}

TEST_F(IoTest, parseMoveValid)
{
    // Test that a valid move does not throw
    ASSERT_NO_THROW(io::parseMove("e2e4", pos));
}

TEST_F(IoTest, tryParseMoveInvalidReturnsNullopt)
{
    EXPECT_FALSE(io::tryParseMove("e2e5", pos).has_value());
    EXPECT_FALSE(io::tryParseMove("bad", pos).has_value());
}

TEST_F(IoTest, tryParseMoveValidReturnsMove)
{
    auto move = io::tryParseMove("e2e4", pos);

    ASSERT_TRUE(move.has_value());
    EXPECT_EQ(board::Move::FROMSQ(move->getValue()), board::toInt(board::Square::E2));
    EXPECT_EQ(board::Move::TOSQ(move->getValue()), board::toInt(board::Square::E4));
    EXPECT_EQ(board::Move::PROMOTED(move->getValue()), board::toInt(board::Piece::EMPTY));
}

TEST_F(IoTest, tryParseMoveRequiresPromotionPiece)
{
    pos.parseFen("8/P7/8/8/8/8/8/4K2k w - - 0 1");

    EXPECT_FALSE(io::tryParseMove("a7a8", pos).has_value());
}

TEST_F(IoTest, tryParseMoveSelectsPromotionPiece)
{
    pos.parseFen("8/P7/8/8/8/8/8/4K2k w - - 0 1");

    auto queenPromotion = io::tryParseMove("a7a8q", pos);
    auto knightPromotion = io::tryParseMove("a7a8n", pos);

    ASSERT_TRUE(queenPromotion.has_value());
    ASSERT_TRUE(knightPromotion.has_value());
    EXPECT_EQ(board::Move::PROMOTED(queenPromotion->getValue()), board::toInt(board::Piece::WQ));
    EXPECT_EQ(board::Move::PROMOTED(knightPromotion->getValue()), board::toInt(board::Piece::WN));
}
