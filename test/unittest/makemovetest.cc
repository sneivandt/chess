#include "board/makemove.h"

#include "board/move.h"
#include "boardtest.h"

using namespace test;

TEST_F(BoardTest, MakeMoveQuiet)
{
    // Start pos
    // e2e4
    int from = board::E2;
    int to = board::E4;
    int moveVal = board::Move::MOVE(from, to, board::EMPTY, board::EMPTY, board::Move::MFLAGPS);
    // MFLAGPS because double pawn push

    board::Move move(moveVal, 0);

    EXPECT_EQ(pos.getSquare(from), board::WP);
    EXPECT_EQ(pos.getSquare(to), board::EMPTY);

    board::makemove::move(move, pos);

    EXPECT_EQ(pos.getSquare(from), board::EMPTY);
    EXPECT_EQ(pos.getSquare(to), board::WP);
    EXPECT_EQ(pos.getSide(), board::BLACK);
    EXPECT_EQ(pos.getEnPas(), board::E3); // En passant square should be E3

    board::makemove::undo(pos);

    EXPECT_EQ(pos.getSquare(from), board::WP);
    EXPECT_EQ(pos.getSquare(to), board::EMPTY);
    EXPECT_EQ(pos.getSide(), board::WHITE);
    EXPECT_EQ(pos.getEnPas(), board::NO_SQ);
}

TEST_F(BoardTest, MakeMoveCapture)
{
    // White to move. N on f3 captures p on e5.
    pos.parseFen("rnbqkbnr/pppp1ppp/8/4p3/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - 1 2");

    int from = board::F3;
    int to = board::E5;
    int capture = board::BP;
    int moveVal = board::Move::MOVE(from, to, capture, board::EMPTY, 0);
    board::Move move(moveVal, 0);

    ASSERT_EQ(pos.getSquare(from), board::WN);
    ASSERT_EQ(pos.getSquare(to), board::BP);

    board::makemove::move(move, pos);

    EXPECT_EQ(pos.getSquare(from), board::EMPTY);
    EXPECT_EQ(pos.getSquare(to), board::WN);
    // captured piece is gone

    board::makemove::undo(pos);
    EXPECT_EQ(pos.getSquare(from), board::WN);
    EXPECT_EQ(pos.getSquare(to), board::BP);
}

TEST_F(BoardTest, MakeMoveCastlingKingsideWhite)
{
    // White to move. Castling available.
    pos.parseFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQK2R w KQkq - 0 1");
    // Setup for castling - empty squares between K and R.
    // Actually start pos has pieces there.
    // The FEN above is RNBQK2R which means e1=K, h1=R. f1, g1 empty?
    // "RNBQK2R" -> R,N,B,Q,K,empty,empty,R.
    // Yes.

    ASSERT_EQ(pos.getSquare(board::E1), board::WK);
    ASSERT_EQ(pos.getSquare(board::H1), board::WR);
    ASSERT_EQ(pos.getSquare(board::F1), board::EMPTY);
    ASSERT_EQ(pos.getSquare(board::G1), board::EMPTY);

    int from = board::E1;
    int to = board::G1;
    int moveVal = board::Move::MOVE(from, to, board::EMPTY, board::EMPTY, board::Move::MFLAGCA);
    board::Move move(moveVal, 0);

    board::makemove::move(move, pos);

    EXPECT_EQ(pos.getSquare(board::E1), board::EMPTY);
    EXPECT_EQ(pos.getSquare(board::G1), board::WK);
    EXPECT_EQ(pos.getSquare(board::F1), board::WR); // Rook moved from H1 to F1
    EXPECT_EQ(pos.getSquare(board::H1), board::EMPTY);

    board::makemove::undo(pos);

    EXPECT_EQ(pos.getSquare(board::E1), board::WK);
    EXPECT_EQ(pos.getSquare(board::H1), board::WR);
    EXPECT_EQ(pos.getSquare(board::F1), board::EMPTY);
    EXPECT_EQ(pos.getSquare(board::G1), board::EMPTY);
}
