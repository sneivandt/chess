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
    int score1 = board::evaluate::score(pos);
    
    // White has non-isolated pawn (with neighboring pawns)
    pos.parseFen("4k3/8/8/8/2PPP3/8/8/4K3 w - - 0 1");
    int score2 = board::evaluate::score(pos);
    
    // Non-isolated pawns should score better (higher) than isolated pawn
    EXPECT_GT(score2, score1);
}

TEST_F(EvaluateTest, PassedPawnBonus)
{
    // White passed pawn on 6th rank
    pos.parseFen("4k3/8/3P4/8/8/8/8/4K3 w - - 0 1");
    int score1 = board::evaluate::score(pos);
    
    // White pawn on 6th rank but blocked by black pawn (not passed)
    pos.parseFen("4k3/3p4/3P4/8/8/8/8/4K3 w - - 0 1");
    int score2 = board::evaluate::score(pos);
    
    // Passed pawn should score better than blocked pawn
    EXPECT_GT(score1, score2);
    // Should have bonus for passed pawn
    EXPECT_GT(score1, 100); // More than just material value
}

TEST_F(EvaluateTest, RookOnOpenFile)
{
    // Test rook on open file gets bonus
    // Open file - no pawns on d-file
    pos.parseFen("3k4/8/8/8/8/8/8/3RK3 w - - 0 1");
    int openFileScore = board::evaluate::score(pos);
    
    // Semi-open file - only white pawn on d-file
    pos.parseFen("3k4/8/8/8/8/8/3P4/3RK3 w - - 0 1");
    int semiOpenFileScore = board::evaluate::score(pos);
    
    // Closed file - both white and black pawns on d-file
    pos.parseFen("3k4/3p4/8/8/8/8/3P4/3RK3 w - - 0 1");
    int closedFileScore = board::evaluate::score(pos);
    
    // Semi-open should be better than closed (despite same material)
    EXPECT_GT(semiOpenFileScore, closedFileScore);
    
    // With extra material, semi-open file position should outscore open file
    EXPECT_GT(semiOpenFileScore, openFileScore);
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
