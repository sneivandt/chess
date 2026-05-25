#include "search/movelist.h"

#include "board/board.h"
#include "unittest.h"

#include <string>
#include <vector>

using namespace test;

class MoveListTest : public test::UnitTest
{
};

TEST_F(MoveListTest, AddMovePreservesInsertionOrder)
{
    search::MoveList list;
    const board::Move e2e4(board::Move::MOVE(board::toInt(board::Square::E2), board::toInt(board::Square::E4),
                                             board::toInt(board::Piece::EMPTY), board::toInt(board::Piece::EMPTY), 0),
                           10);
    const board::Move g1f3(board::Move::MOVE(board::toInt(board::Square::G1), board::toInt(board::Square::F3),
                                             board::toInt(board::Piece::EMPTY), board::toInt(board::Piece::EMPTY), 0),
                           20);

    list.addMove(e2e4);
    list.addMove(g1f3);

    const std::vector<board::Move>& moves = list.getMoves();
    ASSERT_EQ(moves.size(), 2);
    EXPECT_EQ(moves[0].getValue(), e2e4.getValue());
    EXPECT_EQ(moves[0].getScore(), e2e4.getScore());
    EXPECT_EQ(moves[1].getValue(), g1f3.getValue());
    EXPECT_EQ(moves[1].getScore(), g1f3.getScore());
}

TEST_F(MoveListTest, DefaultListIsEmpty)
{
    const search::MoveList list;

    EXPECT_TRUE(list.getMoves().empty());
}

TEST_F(MoveListTest, PrintIncludesAllMoves)
{
    search::MoveList list;
    list.addMove(board::Move(board::Move::MOVE(board::toInt(board::Square::E2), board::toInt(board::Square::E4),
                                               board::toInt(board::Piece::EMPTY), board::toInt(board::Piece::EMPTY), 0),
                             0));
    list.addMove(board::Move(board::Move::MOVE(board::toInt(board::Square::G1), board::toInt(board::Square::F3),
                                               board::toInt(board::Piece::EMPTY), board::toInt(board::Piece::EMPTY), 0),
                             0));

    list.print();

    const std::string output = buffer.str();
    EXPECT_NE(output.find("Move List"), std::string::npos);
    EXPECT_NE(output.find("1 e2e4"), std::string::npos);
    EXPECT_NE(output.find("2 g1f3"), std::string::npos);
}
