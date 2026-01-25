#include "gtest/gtest.h"
#include "board/board.h"
#include "search/movegen.h"
#include "search/movelist.h"
#include "unittest.h"

using namespace test;

class MoveGenTest : public test::UnitTest
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

TEST_F(MoveGenTest, GenerateStartPos)
{
    pos.parseFen(board::Board::DEFAULT_FEN);
    search::MoveList moves = search::movegen::generateAll(pos, false);
    EXPECT_EQ(moves.getMoves().size(), 20);
}

TEST_F(MoveGenTest, GenerateOnlyCapturesStartPos)
{
    pos.parseFen(board::Board::DEFAULT_FEN);
    search::MoveList moves = search::movegen::generateAll(pos, true);
    EXPECT_EQ(moves.getMoves().size(), 0);
}

TEST_F(MoveGenTest, GenerateKnightCapture)
{
    // White N on f3, Black p on e5. N captures p.
    pos.parseFen("rnbqkbnr/pppp1ppp/8/4p3/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - 1 2");

    search::MoveList captures = search::movegen::generateAll(pos, true);
    EXPECT_EQ(captures.getMoves().size(), 1);

    if (captures.getMoves().size() > 0) {
        board::Move m = captures.getMoves()[0];
        EXPECT_EQ(board::Move::FROMSQ(m.getValue()), board::F3);
        EXPECT_EQ(board::Move::TOSQ(m.getValue()), board::E5);
        EXPECT_EQ(board::Move::CAPTURED(m.getValue()), board::BP);
    }
}
