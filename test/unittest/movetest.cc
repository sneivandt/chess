#include "gtest/gtest.h"
#include "board/board.h"
#include "board/move.h"
#include "unittest.h"

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
    int moveVal = board::Move::MOVE(board::E2, board::E4, board::EMPTY, board::EMPTY, 0);
    board::Move move(moveVal, 0);
    
    EXPECT_EQ(move.getValue(), moveVal);
    EXPECT_EQ(move.getScore(), 0);
}

TEST_F(MoveTest, MoveFROMSQ)
{
    int moveVal = board::Move::MOVE(board::E2, board::E4, board::EMPTY, board::EMPTY, 0);
    EXPECT_EQ(board::Move::FROMSQ(moveVal), board::E2);
}

TEST_F(MoveTest, MoveTOSQ)
{
    int moveVal = board::Move::MOVE(board::E2, board::E4, board::EMPTY, board::EMPTY, 0);
    EXPECT_EQ(board::Move::TOSQ(moveVal), board::E4);
}

TEST_F(MoveTest, MoveCAPTURED)
{
    int moveVal = board::Move::MOVE(board::E4, board::D5, board::BP, board::EMPTY, 0);
    EXPECT_EQ(board::Move::CAPTURED(moveVal), board::BP);
}

TEST_F(MoveTest, MovePROMOTED)
{
    int moveVal = board::Move::MOVE(board::E7, board::E8, board::EMPTY, board::WQ, 0);
    EXPECT_EQ(board::Move::PROMOTED(moveVal), board::WQ);
}

TEST_F(MoveTest, MoveGetStringSimple)
{
    // e2e4
    int moveVal = board::Move::MOVE(board::E2, board::E4, board::EMPTY, board::EMPTY, 0);
    board::Move move(moveVal, 0);
    EXPECT_EQ(move.getString(), "e2e4");
}

TEST_F(MoveTest, MoveGetStringCapture)
{
    // Nf3 captures e5
    int moveVal = board::Move::MOVE(board::F3, board::E5, board::BP, board::EMPTY, 0);
    board::Move move(moveVal, 0);
    EXPECT_EQ(move.getString(), "f3e5");
}

TEST_F(MoveTest, MoveGetStringPromotion)
{
    // e7e8 promote to queen
    int moveVal = board::Move::MOVE(board::E7, board::E8, board::EMPTY, board::WQ, 0);
    board::Move move(moveVal, 0);
    EXPECT_EQ(move.getString(), "e7e8Q");
}

TEST_F(MoveTest, MoveGetStringPromotionKnight)
{
    // a7a8 promote to knight
    int moveVal = board::Move::MOVE(board::A7, board::A8, board::EMPTY, board::WN, 0);
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
    int moveVal = board::Move::MOVE(board::E5, board::D6, board::BP, board::EMPTY, board::Move::MFLAGEP);
    EXPECT_TRUE(moveVal & board::Move::MFLAGEP);
    
    // Test pawn start flag
    moveVal = board::Move::MOVE(board::E2, board::E4, board::EMPTY, board::EMPTY, board::Move::MFLAGPS);
    EXPECT_TRUE(moveVal & board::Move::MFLAGPS);
    
    // Test castle flag
    moveVal = board::Move::MOVE(board::E1, board::G1, board::EMPTY, board::EMPTY, board::Move::MFLAGCA);
    EXPECT_TRUE(moveVal & board::Move::MFLAGCA);
}
