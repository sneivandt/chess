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
    
    // Make a move and undo to create same position
    auto moves = search::movegen::generateAll(pos, false).getMoves();
    if (!moves.empty()) {
        board::Move firstMove = moves[0];
        if (board::makemove::move(firstMove, pos)) {
            board::makemove::undo(pos);
            // After undo, should not be repetition (need 3 occurrences)
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

TEST_F(SearchTest, InfinityConstants)
{
    // Ensure infinity constants are properly defined
    EXPECT_LT(search::NEG_INFINITY, 0);
    EXPECT_GT(search::POS_INFINITY, 0);
    // Both should be large values
    EXPECT_LT(search::NEG_INFINITY, -1000000);
    EXPECT_GT(search::POS_INFINITY, 1000000);
}
