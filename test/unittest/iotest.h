#ifndef IOTEST_H
#define IOTEST_H

#include "board/board.h"
#include "search/movegen.h"
#include "unittest.h"

namespace test {

class IoTest : public test::UnitTest
{
  protected:
    board::Board pos;

    virtual void SetUp()
    {
        UnitTest::SetUp();

        board::Board::INIT();
        search::movegen::INIT();
        pos.parseFen(board::Board::DEFAULT_FEN);
    }

    virtual void TearDown()
    {
        UnitTest::TearDown();
    }
};

} // namespace test

#endif
