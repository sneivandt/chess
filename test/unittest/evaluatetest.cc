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
    // Test rook on open file vs semi-open vs closed file
    // Note: Material differences will dominate positional bonuses
    
    // Open file - no pawns on d-file
    pos.parseFen("3k4/8/8/8/8/8/8/3RK3 w - - 0 1");
    int openFileScore = board::evaluate::score(pos);
    
    // Semi-open file - only white pawn on d-file (extra material but worse position)
    pos.parseFen("3k4/8/8/8/8/8/3P4/3RK3 w - - 0 1");
    int semiOpenFileScore = board::evaluate::score(pos);
    
    // Closed file - both white and black pawns on d-file (equal material but worst position)
    pos.parseFen("3k4/3p4/8/8/8/8/3P4/3RK3 w - - 0 1");
    int closedFileScore = board::evaluate::score(pos);
    
    // Semi-open should be better than closed (same material, better position)
    EXPECT_GT(semiOpenFileScore, closedFileScore);
    
    // Semi-open has extra material so should outscore open file
    // (material advantage > positional advantage)
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

TEST_F(EvaluateTest, PassedPawnOnAllRanks)
{
    // Test that passed pawn evaluation works correctly for all ranks
    // This tests the fix for the array bounds bug where RANKS[square] was used
    // with 120-square index instead of 64-square index
    
    // Test passed pawns on different ranks (2nd through 7th)
    pos.parseFen("4k3/8/8/8/8/8/4P3/4K3 w - - 0 1");  // Rank 2
    int score2 = board::evaluate::score(pos);
    EXPECT_GT(score2, 0);  // White should be ahead
    
    pos.parseFen("4k3/8/8/8/8/4P3/8/4K3 w - - 0 1");  // Rank 3
    int score3 = board::evaluate::score(pos);
    EXPECT_GT(score3, score2);  // Advanced pawn should be better
    
    pos.parseFen("4k3/8/8/8/4P3/8/8/4K3 w - - 0 1");  // Rank 4
    int score4 = board::evaluate::score(pos);
    EXPECT_GT(score4, score3);
    
    pos.parseFen("4k3/8/8/4P3/8/8/8/4K3 w - - 0 1");  // Rank 5
    int score5 = board::evaluate::score(pos);
    EXPECT_GT(score5, score4);
    
    pos.parseFen("4k3/8/4P3/8/8/8/8/4K3 w - - 0 1");  // Rank 6
    int score6 = board::evaluate::score(pos);
    EXPECT_GT(score6, score5);
    
    pos.parseFen("4k3/4P3/8/8/8/8/8/4K3 w - - 0 1");  // Rank 7
    int score7 = board::evaluate::score(pos);
    EXPECT_GT(score7, score6);  // 7th rank pawn is most valuable
}

TEST_F(EvaluateTest, RookAndQueenOnAllFiles)
{
    // Test that rook and queen evaluation works correctly for all files
    // This tests the fix for the array bounds bug where FILES[square] was used
    // with 120-square index instead of 64-square index
    
    // Test rooks on different files (a through h)
    pos.parseFen("4k3/8/8/8/8/8/8/R3K3 w - - 0 1");  // File A
    int scoreA = board::evaluate::score(pos);
    EXPECT_GT(scoreA, 0);
    
    pos.parseFen("4k3/8/8/8/8/8/8/4K2R w - - 0 1");  // File H
    int scoreH = board::evaluate::score(pos);
    EXPECT_GT(scoreH, 0);
    
    // Test queen on different files
    pos.parseFen("4k3/8/8/8/8/8/8/Q3K3 w - - 0 1");  // File A
    int queenScoreA = board::evaluate::score(pos);
    EXPECT_GT(queenScoreA, scoreA);  // Queen is more valuable
    
    pos.parseFen("4k3/8/8/8/8/8/8/4K2Q w - - 0 1");  // File H
    int queenScoreH = board::evaluate::score(pos);
    EXPECT_GT(queenScoreH, scoreH);
}
