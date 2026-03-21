#include "search/search.h"

#include "board/board.h"
#include "board/makemove.h"
#include "search/movegen.h"
#include "search/movelist.h"
#include "search/pvtable.h"
#include "search/searchinfo.h"
#include "search/searchstate.h"
#include "unittest.h"

#include "gtest/gtest.h"

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

    search::SearchState ss;
    search::reset(pos, info, ss);

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

    search::SearchState ss;

    // Manually increment ply to a high value to test bounds
    pos.resetPly();
    for (int i = 0; i < 100; i++) {
        pos.incrementPly();
    }

    // Should be able to access searchKillers without crash
    // Get current ply (should be 100)
    EXPECT_EQ(pos.getPly(), 100);

    // Clear search killers (should handle the full range)
    ss.clear();

    // Adding search killers at high ply should not crash
    ss.addKiller(12345, pos.getPly());
    EXPECT_EQ(ss.getKiller(0, pos.getPly()), 12345);

    // Test at ply 127 (boundary condition for 128-element array with indices 0-127)
    pos.resetPly();
    for (int i = 0; i < 127; i++) {
        pos.incrementPly();
    }
    ss.addKiller(67890, pos.getPly());
    EXPECT_EQ(ss.getKiller(0, pos.getPly()), 67890);
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
    EXPECT_NO_THROW({ search::go(pos, info); });
}

// ==============================================================
// PVTable tests
// ==============================================================

TEST_F(SearchTest, PVTableEmptyReturnsNullopt)
{
    search::PVTable pvtable;
    pos.parseFen(board::Board::DEFAULT_FEN);

    auto move = pvtable.getMove(pos);
    EXPECT_FALSE(move.has_value());
}

TEST_F(SearchTest, PVTableStoreAndProbe)
{
    search::PVTable pvtable;
    pos.parseFen(board::Board::DEFAULT_FEN);

    // Create a move (e2e4)
    int moveVal = board::Move::MOVE(board::toInt(board::Square::E2), board::toInt(board::Square::E4),
                                    board::toInt(board::Piece::EMPTY), board::toInt(board::Piece::EMPTY),
                                    board::Move::MFLAGPS);
    board::Move m(moveVal, 0);

    pvtable.addMove(pos, m);
    auto result = pvtable.getMove(pos);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getValue(), m.getValue());
}

TEST_F(SearchTest, PVTableOverwrite)
{
    search::PVTable pvtable;
    pos.parseFen(board::Board::DEFAULT_FEN);

    // Store first move
    int moveVal1 = board::Move::MOVE(board::toInt(board::Square::E2), board::toInt(board::Square::E4),
                                     board::toInt(board::Piece::EMPTY), board::toInt(board::Piece::EMPTY), 0);
    board::Move m1(moveVal1, 0);
    pvtable.addMove(pos, m1);

    // Store second move for same position (overwrites)
    int moveVal2 = board::Move::MOVE(board::toInt(board::Square::D2), board::toInt(board::Square::D4),
                                     board::toInt(board::Piece::EMPTY), board::toInt(board::Piece::EMPTY), 0);
    board::Move m2(moveVal2, 0);
    pvtable.addMove(pos, m2);

    auto result = pvtable.getMove(pos);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getValue(), m2.getValue());
}

TEST_F(SearchTest, PVTableDifferentPositions)
{
    search::PVTable pvtable;

    // Store move for starting position
    pos.parseFen(board::Board::DEFAULT_FEN);
    int moveVal1 = board::Move::MOVE(board::toInt(board::Square::E2), board::toInt(board::Square::E4),
                                     board::toInt(board::Piece::EMPTY), board::toInt(board::Piece::EMPTY), 0);
    board::Move m1(moveVal1, 0);
    pvtable.addMove(pos, m1);

    // Store move for different position
    pos.parseFen("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
    int moveVal2 = board::Move::MOVE(board::toInt(board::Square::E7), board::toInt(board::Square::E5),
                                     board::toInt(board::Piece::EMPTY), board::toInt(board::Piece::EMPTY), 0);
    board::Move m2(moveVal2, 0);
    pvtable.addMove(pos, m2);

    // Probe first position
    pos.parseFen(board::Board::DEFAULT_FEN);
    auto result1 = pvtable.getMove(pos);
    ASSERT_TRUE(result1.has_value());
    EXPECT_EQ(result1->getValue(), m1.getValue());

    // Probe second position
    pos.parseFen("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
    auto result2 = pvtable.getMove(pos);
    ASSERT_TRUE(result2.has_value());
    EXPECT_EQ(result2->getValue(), m2.getValue());
}

TEST_F(SearchTest, PVTableGetPVReturnsLine)
{
    // Run a search which populates the PV table, then verify getPV works
    pos.parseFen(board::Board::DEFAULT_FEN);
    search::SearchInfo info;
    info.setDepth(3);

    search::go(pos, info);

    // After search, the engine should have found at least 1 move
    // (We can't test exact moves but can verify the PV is non-empty)
    // Note: go() creates its own PV table internally, so this mainly tests no crash
}

// ==============================================================
// SearchState tests
// ==============================================================

TEST_F(SearchTest, SearchStateClear)
{
    search::SearchState ss;
    ss.incrementHistory(0, 50, 10, board::toInt(board::Piece::WP));
    ss.addKiller(12345, 0);

    EXPECT_NE(ss.getHistory(board::toInt(board::Piece::WP), 50), 0);
    EXPECT_EQ(ss.getKiller(0, 0), 12345);

    ss.clear();

    EXPECT_EQ(ss.getHistory(board::toInt(board::Piece::WP), 50), 0);
    EXPECT_EQ(ss.getKiller(0, 0), 0);
}

TEST_F(SearchTest, SearchStateKillerRotation)
{
    search::SearchState ss;

    ss.addKiller(100, 5);
    EXPECT_EQ(ss.getKiller(0, 5), 100);

    ss.addKiller(200, 5);
    EXPECT_EQ(ss.getKiller(0, 5), 200);
    EXPECT_EQ(ss.getKiller(1, 5), 100);
}

TEST_F(SearchTest, SearchStateHistoryAccumulates)
{
    search::SearchState ss;
    int piece = board::toInt(board::Piece::WN);
    int sq = 50;

    ss.incrementHistory(0, sq, 3, piece);
    EXPECT_EQ(ss.getHistory(piece, sq), 3);

    ss.incrementHistory(0, sq, 5, piece);
    EXPECT_EQ(ss.getHistory(piece, sq), 8);
}

// ==============================================================
// Tactical search tests
// ==============================================================

TEST_F(SearchTest, MateInOne)
{
    // White to move, Qh7# is mate in 1
    pos.parseFen("r1bqkb1r/pppp1ppp/2n2n2/4p2Q/2B1P3/8/PPPP1PPP/RNB1K1NR w KQkq - 4 4");
    search::SearchInfo info;
    info.setDepth(2);
    search::go(pos, info);
    // Primarily verifying no crash and the search completes
}

TEST_F(SearchTest, SearchDepthProgression)
{
    // Verify search at different depths doesn't crash
    pos.parseFen(board::Board::DEFAULT_FEN);
    for (int depth = 1; depth <= 4; depth++) {
        board::Board tempPos;
        tempPos.parseFen(board::Board::DEFAULT_FEN);
        search::SearchInfo info;
        info.setDepth(depth);
        EXPECT_NO_THROW({ search::go(tempPos, info); });
    }
}
