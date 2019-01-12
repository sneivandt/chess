#ifndef BOARDTEST_H
#define BOARDTEST_H

#include "unittest/unittest.h"

#include "board/board.h"

namespace test {

class BoardTest : public test::UnitTest
{
  protected:
    board::Board pos;

    virtual void SetUp()
    {
        UnitTest::SetUp();

        board::Board::INIT();
        pos.parseFen(board::Board::DEFAULT_FEN);
    }

    virtual void TearDown()
    {
        UnitTest::TearDown();
    }

    void ASSERT_BOARD(board::Board, std::map<board::Square, board::Piece>, board::Color, board::Square, int,
                      int = board::WKCA | board::WQCA | board::BKCA | board::BQCA);
};

} // namespace test

#endif