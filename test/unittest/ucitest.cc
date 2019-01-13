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