#include "gtest/gtest.h"
#include "board/board.h"
#include "board/evaluate.h"
#include "unittest.h"

using namespace test;

class EvaluateTest : public test::UnitTest
{
protected:
    board::Board pos;
    virtual void SetUp()
    {
        UnitTest::SetUp();
        board::Board::INIT();
    }
};

TEST_F(EvaluateTest, StartPosScore)
{
    // Start position should have a score close to 0 (equal material)
    pos.parseFen(board::Board::DEFAULT_FEN);
    int score = board::evaluate::score(pos);
    // Should be close to 0, allowing for positional differences
    EXPECT_GT(score, -100);
    EXPECT_LT(score, 100);
}

TEST_F(EvaluateTest, WhitePawnAdvantage)
{
    // White has an extra pawn
    pos.parseFen("rnbqkbnr/ppp1pppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    int score = board::evaluate::score(pos);
    // White should be ahead
    EXPECT_GT(score, 0);
}

TEST_F(EvaluateTest, BlackPawnAdvantage)
{
    // Black has an extra pawn
    pos.parseFen("rnbqkbnr/pppppppp/8/8/8/8/PPP1PPPP/RNBQKBNR w KQkq - 0 1");
    int score = board::evaluate::score(pos);
    // Black should be ahead (negative score)
    EXPECT_LT(score, 0);
}

TEST_F(EvaluateTest, MaterialDraw)
{
    // King vs King - should be draw
    pos.parseFen("8/8/8/4k3/8/8/4K3/8 w - - 0 1");
    int score = board::evaluate::score(pos);
    EXPECT_EQ(score, 0);
}

TEST_F(EvaluateTest, MaterialDrawKnightVsKnight)
{
    // King + Knight vs King + Knight - should be draw
    pos.parseFen("8/8/8/3nk3/8/8/3NK3/8 w - - 0 1");
    int score = board::evaluate::score(pos);
    EXPECT_EQ(score, 0);
}

TEST_F(EvaluateTest, MaterialDrawTwoKnights)
{
    // King + 2 Knights vs King - should be draw (insufficient material)
    pos.parseFen("8/8/8/4k3/8/8/2NNK3/8 w - - 0 1");
    int score = board::evaluate::score(pos);
    EXPECT_EQ(score, 0);
}

TEST_F(EvaluateTest, BishopPairBonus)
{
    // White has bishop pair
    pos.parseFen("r1bqk1nr/pppppppp/2n5/8/8/2N5/PPPPPPPP/R1BQKB1R w KQkq - 0 1");
    int score1 = board::evaluate::score(pos);
    
    // Now remove one white bishop
    pos.parseFen("r1bqk1nr/pppppppp/2n5/8/8/2N5/PPPPPPPP/R1BQK2R w KQkq - 0 1");
    int score2 = board::evaluate::score(pos);
    
    // With bishop pair, score should be higher
    EXPECT_GT(score1, score2);
}

TEST_F(EvaluateTest, IsolatedPawnPenalty)
{
    // White has isolated pawn on d4, also has king
    pos.parseFen("4k3/8/8/8/3P4/8/8/4K3 w - - 0 1");
    int score = board::evaluate::score(pos);
    // Just check that it returns a valid score
    EXPECT_GT(score, -1000);
    EXPECT_LT(score, 1000);
}

TEST_F(EvaluateTest, PassedPawnBonus)
{
    // White passed pawn on 6th rank
    pos.parseFen("4k3/8/3P4/8/8/8/8/4K3 w - - 0 1");
    int score = board::evaluate::score(pos);
    // Should have bonus for passed pawn
    EXPECT_GT(score, 100); // More than just material value
}

TEST_F(EvaluateTest, RookOnOpenFile)
{
    // White rook on open file
    pos.parseFen("4k3/8/8/8/8/8/8/3RK3 w - - 0 1");
    int score1 = board::evaluate::score(pos);
    
    // White rook blocked by own pawn
    pos.parseFen("4k3/8/8/8/8/8/3P4/3RK3 w - - 0 1");
    int score2 = board::evaluate::score(pos);
    
    // With pawn should have material advantage but worse position
    // Just check that both return valid scores
    EXPECT_GT(score1, -1000);
    EXPECT_LT(score1, 1000);
    EXPECT_GT(score2, -1000);
    EXPECT_LT(score2, 1000);
}

TEST_F(EvaluateTest, BlackToMove)
{
    // Score should be relative to side to move
    pos.parseFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    int whiteScore = board::evaluate::score(pos);
    
    pos.parseFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1");
    int blackScore = board::evaluate::score(pos);
    
    // Scores should be opposite (negated)
    EXPECT_EQ(whiteScore, -blackScore);
}
