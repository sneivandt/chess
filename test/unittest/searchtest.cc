#include "gtest/gtest.h"
#include "board/board.h"
#include "board/makemove.h"
#include "search/search.h"
#include "search/searchinfo.h"
#include "search/movegen.h"
#include "search/movelist.h"
#include "unittest.h"

using namespace test;

class SearchTest : public test::UnitTest
{
protected:
    board::Board pos;
    virtual void SetUp()
    {
        UnitTest::SetUp();
        board::Board::INIT();
        search::movegen::INIT();
    }
};

TEST_F(SearchTest, IsRepetition)
{
    // Setup position
    pos.parseFen(board::Board::DEFAULT_FEN);
    
    // Initially no repetition
    EXPECT_FALSE(search::isRepetition(pos));
}

TEST_F(SearchTest, IsRepetitionAfterMoves)
{
    // Setup position and make moves that repeat
    pos.parseFen("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");
    
    // Make a move and undo to return to the same position
    auto moves = search::movegen::generateAll(pos, false).getMoves();
    if (!moves.empty()) {
        board::Move firstMove = moves[0];
        if (board::makemove::move(firstMove, pos)) {
            board::makemove::undo(pos);
            // After undo, this position has only a single occurrence in history, so isRepetition should be false
            EXPECT_FALSE(search::isRepetition(pos));
        }
    }
}

TEST_F(SearchTest, SearchInfoInitialization)
{
    search::SearchInfo info;
    
    // Check defaults
    EXPECT_EQ(info.getDepth(), -1);
    EXPECT_FALSE(info.getStopped());
    EXPECT_FALSE(info.getQuit());
    EXPECT_EQ(info.getNodes(), 0);
}

TEST_F(SearchTest, SearchInfoSetters)
{
    search::SearchInfo info;
    
    info.setDepth(5);
    EXPECT_EQ(info.getDepth(), 5);
    
    info.setStopped(true);
    EXPECT_TRUE(info.getStopped());
    
    info.setQuit(true);
    EXPECT_TRUE(info.getQuit());
    
    info.incrementNodes();
    EXPECT_EQ(info.getNodes(), 1);
    
    info.incrementNodes();
    EXPECT_EQ(info.getNodes(), 2);
}

TEST_F(SearchTest, SearchReset)
{
    search::SearchInfo info;
    
    info.setDepth(5);
    info.setStopped(true);
    info.incrementNodes();
    
    search::reset(pos, info);
    
    // After reset, search info should be reset
    EXPECT_FALSE(info.getStopped());
    EXPECT_EQ(info.getNodes(), 0);
}

TEST_F(SearchTest, IsRepetitionWithHighFiftyMoveCounter)
{
    // Test edge case where fifty-move counter exceeds history size
    // This tests the fix for unsigned underflow in isRepetition()
    
    // Parse FEN with high fifty-move counter (50 half-moves)
    pos.parseFen("4k3/8/8/8/8/8/8/4K3 w - - 50 26");
    
    // With empty or small history, this should not crash or cause underflow
    EXPECT_FALSE(search::isRepetition(pos));
    
    // Make a move to add to history
    auto moves = search::movegen::generateAll(pos, false).getMoves();
    if (!moves.empty()) {
        if (board::makemove::move(moves[0], pos)) {
            // Still should not crash
            EXPECT_FALSE(search::isRepetition(pos));
        }
    }
}

TEST_F(SearchTest, InfinityConstants)
{
    // Ensure infinity constants are properly defined
    EXPECT_LT(search::NEG_INFINITY, 0);
    EXPECT_GT(search::POS_INFINITY, 0);
    // Both should be large values
    EXPECT_LT(search::NEG_INFINITY, -1000000);
    EXPECT_GT(search::POS_INFINITY, 1000000);
}

// Test for Bug Fix 1: searchKillers array bounds with deep ply
TEST_F(SearchTest, SearchKillersDeepPly)
{
    // Test that searchKillers can handle ply values up to 127
    pos.parseFen(board::Board::DEFAULT_FEN);
    
    // Manually increment ply to a high value to test bounds
    pos.resetPly();
    for (int i = 0; i < 100; i++) {
        pos.incrementPly();
    }
    
    // Should be able to access searchKillers without crash
    // Get current ply (should be 100)
    EXPECT_EQ(pos.getPly(), 100);
    
    // Clear search killers (should handle the full range)
    pos.clearSearchKillers();
    
    // Adding search killers at high ply should not crash
    pos.addSearchKiller(12345);
    EXPECT_EQ(pos.getSearchKiller(0), 12345);
    
    // Test at ply 127 (boundary condition for 128-element array with indices 0-127)
    pos.resetPly();
    for (int i = 0; i < 127; i++) {
        pos.incrementPly();
    }
    pos.addSearchKiller(67890);
    EXPECT_EQ(pos.getSearchKiller(0), 67890);
}

// Test for Bug Fix 3: isRepetition with empty history
TEST_F(SearchTest, IsRepetitionEmptyHistory)
{
    // Test that isRepetition handles empty history gracefully
    pos.parseFen(board::Board::DEFAULT_FEN);
    
    // Initial position has empty history
    EXPECT_FALSE(search::isRepetition(pos));
    
    // Also test with fiftyMove = 0
    pos.parseFen("4k3/8/8/8/8/8/8/4K3 w - - 0 1");
    EXPECT_FALSE(search::isRepetition(pos));
}

// Test that search handles valid positions correctly
TEST_F(SearchTest, SearchValidPosition)
{
    pos.parseFen(board::Board::DEFAULT_FEN);
    search::SearchInfo info;
    info.setDepth(1);
    
    // Should not crash with valid position containing kings
    EXPECT_NO_THROW({
        search::go(pos, info);
    });
}

