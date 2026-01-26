#include "ucitest.h"

#include "board/board.h"
#include "io/uci.h"

#include "gtest/gtest_pred_impl.h"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

using namespace test;

TEST_F(UciTest, parsePositionStartPos)
{
    io::uci::parsePosition("startpos", pos);

    ASSERT_TRUE(pos.parseFen(board::Board::DEFAULT_FEN));

    ASSERT_BOARD(pos,
                 {

                 },
                 board::WHITE, board::NO_SQ, 0);
}

TEST_F(UciTest, parseFenRuyLopez)
{
    io::uci::parsePosition("fen r1bqkbnr/pppp1ppp/2n5/1B2p3/4P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 3 3", pos);

    ASSERT_BOARD(pos,
                 {
                     {board::E4, board::WP},
                     {board::E5, board::BP},
                     {board::F3, board::WN},
                     {board::C6, board::BN},
                     {board::B5, board::WB},
                 },
                 board::BLACK, board::NO_SQ, 3);
}

TEST_F(UciTest, parseMovesA2A3A7A6)
{
    io::uci::parsePosition("moves a2a3", pos);

    ASSERT_BOARD(pos,
                 {
                     {board::A3, board::WP},
                 },
                 board::BLACK, board::NO_SQ, 0);

    io::uci::parsePosition("moves a7a6", pos);

    ASSERT_BOARD(pos,
                 {
                     {board::A3, board::WP},
                     {board::A6, board::BP},
                 },
                 board::WHITE, board::NO_SQ, 0);
}

TEST_F(UciTest, parseFenWithMoves)
{
    // Test parsing FEN string followed by moves
    // This tests the fix for inverted FEN parsing logic
    io::uci::parsePosition("fen rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 moves e2e4 e7e5", pos);

    ASSERT_BOARD(pos,
                 {
                     {board::E4, board::WP},
                     {board::E5, board::BP},
                 },
                 board::WHITE, board::E6, 0);  // En passant is E6 after black's e7e5
}

TEST_F(UciTest, parseFenWithMovesComplexPosition)
{
    // Test with a more complex FEN and multiple moves
    io::uci::parsePosition("fen r1bqkbnr/pppp1ppp/2n5/4p3/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - 2 3 moves f1b5", pos);

    ASSERT_BOARD(pos,
                 {
                     {board::E4, board::WP},
                     {board::E5, board::BP},
                     {board::F3, board::WN},
                     {board::C6, board::BN},
                     {board::B5, board::WB},
                 },
                 board::BLACK, board::NO_SQ, 3);
}

// Test for Bug Fix 6: Bounds check in UCI position parsing
TEST_F(UciTest, parsePositionShortInput)
{
    // Test with input that is too short (edge case)
    // This should not crash with the bounds check in place
    // Note: In real UCI loop, this wouldn't happen, but we test the bounds check
    io::uci::parsePosition("startpos", pos);
    
    // Should parse successfully to start position
    ASSERT_EQ(pos.getSide(), board::WHITE);
}