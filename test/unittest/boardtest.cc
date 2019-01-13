#include "boardtest.h"

#include "gtest/gtest_pred_impl.h"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <utility>

using namespace test;

void BoardTest::ASSERT_BOARD(const board::Board& pos, std::map<board::Square, board::Piece> pieces, board::Color side,
                             board::Square enPas, int fiftyMove, int castlePerm)
{
    ASSERT_EQ(pos.getSide(), side);
    ASSERT_EQ(pos.getEnPas(), enPas);
    ASSERT_EQ(pos.getFiftyMove(), fiftyMove);
    ASSERT_EQ(pos.getCastlePerm(), castlePerm);

    for (auto const& p : pieces) {
        ASSERT_EQ(pos.getSquare(p.first), p.second);
    }
}

TEST_F(BoardTest, parseFenInvalid)
{
    ASSERT_FALSE(pos.parseFen("foo"));
}

TEST_F(BoardTest, parseFenStartPos)
{
    ASSERT_TRUE(pos.parseFen(board::Board::DEFAULT_FEN));

    ASSERT_BOARD(pos,
                 {

                 },
                 board::WHITE, board::NO_SQ, 0);
}

TEST_F(BoardTest, parseFenRuyLopez)
{
    ASSERT_TRUE(pos.parseFen("r1bqkbnr/pppp1ppp/2n5/1B2p3/4P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 3 3"));

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

TEST_F(BoardTest, parseFenSicilianDefence)
{
    ASSERT_TRUE(pos.parseFen("rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 0 2"));

    ASSERT_BOARD(pos,
                 {
                     {board::E4, board::WP},
                     {board::C5, board::BP},
                 },
                 board::WHITE, board::NO_SQ, 0);
}