#include "board/move.h"

#include "board/board.h"
#include "unittest.h"

#include "gtest/gtest.h"

using namespace test;

class MoveTest : public test::UnitTest
{
  protected:
    virtual void SetUp()
    {
        UnitTest::SetUp();
        board::Board::INIT();
    }
};

TEST_F(MoveTest, MoveConstruction)
{
    // Create a simple move
    int moveVal = board::Move::MOVE(board::toInt(board::Square::E2), board::toInt(board::Square::E4),
                                    board::toInt(board::Piece::EMPTY), board::toInt(board::Piece::EMPTY), 0);
    board::Move move(moveVal, 0);

    EXPECT_EQ(move.getValue(), moveVal);
    EXPECT_EQ(move.getScore(), 0);
}

TEST_F(MoveTest, MoveFROMSQ)
{
    int moveVal = board::Move::MOVE(board::toInt(board::Square::E2), board::toInt(board::Square::E4),
                                    board::toInt(board::Piece::EMPTY), board::toInt(board::Piece::EMPTY), 0);
    EXPECT_EQ(board::Move::FROMSQ(moveVal), board::toInt(board::Square::E2));
}

TEST_F(MoveTest, MoveTOSQ)
{
    int moveVal = board::Move::MOVE(board::toInt(board::Square::E2), board::toInt(board::Square::E4),
                                    board::toInt(board::Piece::EMPTY), board::toInt(board::Piece::EMPTY), 0);
    EXPECT_EQ(board::Move::TOSQ(moveVal), board::toInt(board::Square::E4));
}

TEST_F(MoveTest, MoveCAPTURED)
{
    int moveVal = board::Move::MOVE(board::toInt(board::Square::E4), board::toInt(board::Square::D5),
                                    board::toInt(board::Piece::BP), board::toInt(board::Piece::EMPTY), 0);
    EXPECT_EQ(board::Move::CAPTURED(moveVal), board::toInt(board::Piece::BP));
}

TEST_F(MoveTest, MovePROMOTED)
{
    int moveVal = board::Move::MOVE(board::toInt(board::Square::E7), board::toInt(board::Square::E8),
                                    board::toInt(board::Piece::EMPTY), board::toInt(board::Piece::WQ), 0);
    EXPECT_EQ(board::Move::PROMOTED(moveVal), board::toInt(board::Piece::WQ));
}

TEST_F(MoveTest, MoveGetStringSimple)
{
    // e2e4
    int moveVal = board::Move::MOVE(board::toInt(board::Square::E2), board::toInt(board::Square::E4),
                                    board::toInt(board::Piece::EMPTY), board::toInt(board::Piece::EMPTY), 0);
    board::Move move(moveVal, 0);
    EXPECT_EQ(move.getString(), "e2e4");
}

TEST_F(MoveTest, MoveGetStringCapture)
{
    // Nf3 captures e5
    int moveVal = board::Move::MOVE(board::toInt(board::Square::F3), board::toInt(board::Square::E5),
                                    board::toInt(board::Piece::BP), board::toInt(board::Piece::EMPTY), 0);
    board::Move move(moveVal, 0);
    EXPECT_EQ(move.getString(), "f3e5");
}

TEST_F(MoveTest, MoveGetStringPromotion)
{
    // e7e8 promote to queen
    int moveVal = board::Move::MOVE(board::toInt(board::Square::E7), board::toInt(board::Square::E8),
                                    board::toInt(board::Piece::EMPTY), board::toInt(board::Piece::WQ), 0);
    board::Move move(moveVal, 0);
    EXPECT_EQ(move.getString(), "e7e8Q");
}

TEST_F(MoveTest, MoveGetStringPromotionKnight)
{
    // a7a8 promote to knight
    int moveVal = board::Move::MOVE(board::toInt(board::Square::A7), board::toInt(board::Square::A8),
                                    board::toInt(board::Piece::EMPTY), board::toInt(board::Piece::WN), 0);
    board::Move move(moveVal, 0);
    EXPECT_EQ(move.getString(), "a7a8N");
}

TEST_F(MoveTest, MoveScoring)
{
    board::Move move(0, 100);
    EXPECT_EQ(move.getScore(), 100);

    move.addScore(50);
    EXPECT_EQ(move.getScore(), 150);
}

TEST_F(MoveTest, MoveComparison)
{
    board::Move move1(0, 100);
    board::Move move2(0, 200);

    // Lower score should be "less than" higher score
    EXPECT_TRUE(move1 < move2);
    EXPECT_FALSE(move2 < move1);
}

TEST_F(MoveTest, MoveFlags)
{
    // Test en passant flag
    int moveVal =
        board::Move::MOVE(board::toInt(board::Square::E5), board::toInt(board::Square::D6),
                          board::toInt(board::Piece::BP), board::toInt(board::Piece::EMPTY), board::Move::MFLAGEP);
    EXPECT_TRUE(moveVal & board::Move::MFLAGEP);

    // Test pawn start flag
    moveVal =
        board::Move::MOVE(board::toInt(board::Square::E2), board::toInt(board::Square::E4),
                          board::toInt(board::Piece::EMPTY), board::toInt(board::Piece::EMPTY), board::Move::MFLAGPS);
    EXPECT_TRUE(moveVal & board::Move::MFLAGPS);

    // Test castle flag
    moveVal =
        board::Move::MOVE(board::toInt(board::Square::E1), board::toInt(board::Square::G1),
                          board::toInt(board::Piece::EMPTY), board::toInt(board::Piece::EMPTY), board::Move::MFLAGCA);
    EXPECT_TRUE(moveVal & board::Move::MFLAGCA);
}
