#include "board/makemove.h"

#include "board/move.h"
#include "boardtest.h"

using namespace test;

TEST_F(BoardTest, MakeMoveQuiet)
{
    // Start pos
    // e2e4
    int from = board::toInt(board::Square::E2);
    int to = board::toInt(board::Square::E4);
    int moveVal = board::Move::MOVE(from, to, board::toInt(board::Piece::EMPTY), board::toInt(board::Piece::EMPTY),
                                    board::Move::MFLAGPS);
    // MFLAGPS because double pawn push

    board::Move move(moveVal, 0);

    EXPECT_EQ(pos.getSquare(from), board::toInt(board::Piece::WP));
    EXPECT_EQ(pos.getSquare(to), board::toInt(board::Piece::EMPTY));

    board::makemove::move(move, pos);

    EXPECT_EQ(pos.getSquare(from), board::toInt(board::Piece::EMPTY));
    EXPECT_EQ(pos.getSquare(to), board::toInt(board::Piece::WP));
    EXPECT_EQ(pos.getSide(), board::toInt(board::Color::BLACK));
    EXPECT_EQ(pos.getEnPas(), board::toInt(board::Square::E3)); // En passant square should be E3

    board::makemove::undo(pos);

    EXPECT_EQ(pos.getSquare(from), board::toInt(board::Piece::WP));
    EXPECT_EQ(pos.getSquare(to), board::toInt(board::Piece::EMPTY));
    EXPECT_EQ(pos.getSide(), board::toInt(board::Color::WHITE));
    EXPECT_EQ(pos.getEnPas(), board::toInt(board::Square::NO_SQ));
}

TEST_F(BoardTest, MakeMoveCapture)
{
    // White to move. N on f3 captures p on e5.
    pos.parseFen("rnbqkbnr/pppp1ppp/8/4p3/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - 1 2");

    int from = board::toInt(board::Square::F3);
    int to = board::toInt(board::Square::E5);
    int capture = board::toInt(board::Piece::BP);
    int moveVal = board::Move::MOVE(from, to, capture, board::toInt(board::Piece::EMPTY), 0);
    board::Move move(moveVal, 0);

    ASSERT_EQ(pos.getSquare(from), board::toInt(board::Piece::WN));
    ASSERT_EQ(pos.getSquare(to), board::toInt(board::Piece::BP));

    board::makemove::move(move, pos);

    EXPECT_EQ(pos.getSquare(from), board::toInt(board::Piece::EMPTY));
    EXPECT_EQ(pos.getSquare(to), board::toInt(board::Piece::WN));
    // captured piece is gone

    board::makemove::undo(pos);
    EXPECT_EQ(pos.getSquare(from), board::toInt(board::Piece::WN));
    EXPECT_EQ(pos.getSquare(to), board::toInt(board::Piece::BP));
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

    ASSERT_EQ(pos.getSquare(board::toInt(board::Square::E1)), board::toInt(board::Piece::WK));
    ASSERT_EQ(pos.getSquare(board::toInt(board::Square::H1)), board::toInt(board::Piece::WR));
    ASSERT_EQ(pos.getSquare(board::toInt(board::Square::F1)), board::toInt(board::Piece::EMPTY));
    ASSERT_EQ(pos.getSquare(board::toInt(board::Square::G1)), board::toInt(board::Piece::EMPTY));

    int from = board::toInt(board::Square::E1);
    int to = board::toInt(board::Square::G1);
    int moveVal = board::Move::MOVE(from, to, board::toInt(board::Piece::EMPTY), board::toInt(board::Piece::EMPTY),
                                    board::Move::MFLAGCA);
    board::Move move(moveVal, 0);

    board::makemove::move(move, pos);

    EXPECT_EQ(pos.getSquare(board::toInt(board::Square::E1)), board::toInt(board::Piece::EMPTY));
    EXPECT_EQ(pos.getSquare(board::toInt(board::Square::G1)), board::toInt(board::Piece::WK));
    EXPECT_EQ(pos.getSquare(board::toInt(board::Square::F1)),
              board::toInt(board::Piece::WR)); // Rook moved from H1 to F1
    EXPECT_EQ(pos.getSquare(board::toInt(board::Square::H1)), board::toInt(board::Piece::EMPTY));

    board::makemove::undo(pos);

    EXPECT_EQ(pos.getSquare(board::toInt(board::Square::E1)), board::toInt(board::Piece::WK));
    EXPECT_EQ(pos.getSquare(board::toInt(board::Square::H1)), board::toInt(board::Piece::WR));
    EXPECT_EQ(pos.getSquare(board::toInt(board::Square::F1)), board::toInt(board::Piece::EMPTY));
    EXPECT_EQ(pos.getSquare(board::toInt(board::Square::G1)), board::toInt(board::Piece::EMPTY));
}

TEST_F(BoardTest, MakeMoveCastlingQueensideWhite)
{
    // White queenside castling
    pos.parseFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/R3KBNR w KQkq - 0 1");

    ASSERT_EQ(pos.getSquare(board::toInt(board::Square::E1)), board::toInt(board::Piece::WK));
    ASSERT_EQ(pos.getSquare(board::toInt(board::Square::A1)), board::toInt(board::Piece::WR));
    ASSERT_EQ(pos.getSquare(board::toInt(board::Square::B1)), board::toInt(board::Piece::EMPTY));
    ASSERT_EQ(pos.getSquare(board::toInt(board::Square::C1)), board::toInt(board::Piece::EMPTY));
    ASSERT_EQ(pos.getSquare(board::toInt(board::Square::D1)), board::toInt(board::Piece::EMPTY));

    int from = board::toInt(board::Square::E1);
    int to = board::toInt(board::Square::C1);
    int moveVal = board::Move::MOVE(from, to, board::toInt(board::Piece::EMPTY), board::toInt(board::Piece::EMPTY),
                                    board::Move::MFLAGCA);
    board::Move move(moveVal, 0);

    board::makemove::move(move, pos);

    EXPECT_EQ(pos.getSquare(board::toInt(board::Square::E1)), board::toInt(board::Piece::EMPTY));
    EXPECT_EQ(pos.getSquare(board::toInt(board::Square::C1)), board::toInt(board::Piece::WK));
    EXPECT_EQ(pos.getSquare(board::toInt(board::Square::D1)),
              board::toInt(board::Piece::WR)); // Rook moved from A1 to D1
    EXPECT_EQ(pos.getSquare(board::toInt(board::Square::A1)), board::toInt(board::Piece::EMPTY));

    board::makemove::undo(pos);

    EXPECT_EQ(pos.getSquare(board::toInt(board::Square::E1)), board::toInt(board::Piece::WK));
    EXPECT_EQ(pos.getSquare(board::toInt(board::Square::A1)), board::toInt(board::Piece::WR));
    EXPECT_EQ(pos.getSquare(board::toInt(board::Square::C1)), board::toInt(board::Piece::EMPTY));
    EXPECT_EQ(pos.getSquare(board::toInt(board::Square::D1)), board::toInt(board::Piece::EMPTY));
}

TEST_F(BoardTest, MakeMoveCastlingKingsideBlack)
{
    // Black kingside castling
    pos.parseFen("rnbqk2r/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1");

    ASSERT_EQ(pos.getSquare(board::toInt(board::Square::E8)), board::toInt(board::Piece::BK));
    ASSERT_EQ(pos.getSquare(board::toInt(board::Square::H8)), board::toInt(board::Piece::BR));

    int from = board::toInt(board::Square::E8);
    int to = board::toInt(board::Square::G8);
    int moveVal = board::Move::MOVE(from, to, board::toInt(board::Piece::EMPTY), board::toInt(board::Piece::EMPTY),
                                    board::Move::MFLAGCA);
    board::Move move(moveVal, 0);

    board::makemove::move(move, pos);

    EXPECT_EQ(pos.getSquare(board::toInt(board::Square::E8)), board::toInt(board::Piece::EMPTY));
    EXPECT_EQ(pos.getSquare(board::toInt(board::Square::G8)), board::toInt(board::Piece::BK));
    EXPECT_EQ(pos.getSquare(board::toInt(board::Square::F8)), board::toInt(board::Piece::BR));
    EXPECT_EQ(pos.getSquare(board::toInt(board::Square::H8)), board::toInt(board::Piece::EMPTY));

    board::makemove::undo(pos);

    EXPECT_EQ(pos.getSquare(board::toInt(board::Square::E8)), board::toInt(board::Piece::BK));
    EXPECT_EQ(pos.getSquare(board::toInt(board::Square::H8)), board::toInt(board::Piece::BR));
}

TEST_F(BoardTest, MakeMoveEnPassant)
{
    // White pawn on e5, black pawn advances from d7 to d5, white can capture en passant
    pos.parseFen("rnbqkbnr/ppp1pppp/8/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 1");

    ASSERT_EQ(pos.getSquare(board::toInt(board::Square::E5)), board::toInt(board::Piece::WP));
    ASSERT_EQ(pos.getSquare(board::toInt(board::Square::D5)), board::toInt(board::Piece::BP));
    ASSERT_EQ(pos.getEnPas(), board::toInt(board::Square::D6));

    int from = board::toInt(board::Square::E5);
    int to = board::toInt(board::Square::D6);
    // En passant moves should have captured = EMPTY, as the capture is handled separately
    int moveVal = board::Move::MOVE(from, to, board::toInt(board::Piece::EMPTY), board::toInt(board::Piece::EMPTY),
                                    board::Move::MFLAGEP);
    board::Move move(moveVal, 0);

    board::makemove::move(move, pos);

    EXPECT_EQ(pos.getSquare(board::toInt(board::Square::E5)), board::toInt(board::Piece::EMPTY));
    EXPECT_EQ(pos.getSquare(board::toInt(board::Square::D6)), board::toInt(board::Piece::WP));
    EXPECT_EQ(pos.getSquare(board::toInt(board::Square::D5)),
              board::toInt(board::Piece::EMPTY)); // Captured pawn removed

    // Test undo
    board::makemove::undo(pos);
    EXPECT_EQ(pos.getSquare(board::toInt(board::Square::E5)), board::toInt(board::Piece::WP));
    EXPECT_EQ(pos.getSquare(board::toInt(board::Square::D5)), board::toInt(board::Piece::BP));
    EXPECT_EQ(pos.getSquare(board::toInt(board::Square::D6)), board::toInt(board::Piece::EMPTY));
}

TEST_F(BoardTest, MakeMovePromotionQueen)
{
    // White pawn promotes to queen
    pos.parseFen("8/4P3/8/8/8/8/8/8 w - - 0 1");

    ASSERT_EQ(pos.getSquare(board::toInt(board::Square::E7)), board::toInt(board::Piece::WP));

    int from = board::toInt(board::Square::E7);
    int to = board::toInt(board::Square::E8);
    int moveVal = board::Move::MOVE(from, to, board::toInt(board::Piece::EMPTY), board::toInt(board::Piece::WQ), 0);
    board::Move move(moveVal, 0);

    board::makemove::move(move, pos);

    EXPECT_EQ(pos.getSquare(board::toInt(board::Square::E7)), board::toInt(board::Piece::EMPTY));
    EXPECT_EQ(pos.getSquare(board::toInt(board::Square::E8)), board::toInt(board::Piece::WQ));

    board::makemove::undo(pos);

    EXPECT_EQ(pos.getSquare(board::toInt(board::Square::E7)), board::toInt(board::Piece::WP));
    EXPECT_EQ(pos.getSquare(board::toInt(board::Square::E8)), board::toInt(board::Piece::EMPTY));
}

TEST_F(BoardTest, MakeMovePromotionKnight)
{
    // White pawn promotes to knight
    pos.parseFen("8/4P3/8/8/8/8/8/8 w - - 0 1");

    int from = board::toInt(board::Square::E7);
    int to = board::toInt(board::Square::E8);
    int moveVal = board::Move::MOVE(from, to, board::toInt(board::Piece::EMPTY), board::toInt(board::Piece::WN), 0);
    board::Move move(moveVal, 0);

    board::makemove::move(move, pos);

    EXPECT_EQ(pos.getSquare(board::toInt(board::Square::E8)), board::toInt(board::Piece::WN));

    board::makemove::undo(pos);

    EXPECT_EQ(pos.getSquare(board::toInt(board::Square::E7)), board::toInt(board::Piece::WP));
}

TEST_F(BoardTest, MakeMovePromotionCapture)
{
    // White pawn captures and promotes
    pos.parseFen("4r3/4P3/8/8/8/8/8/8 w - - 0 1");

    ASSERT_EQ(pos.getSquare(board::toInt(board::Square::E7)), board::toInt(board::Piece::WP));
    ASSERT_EQ(pos.getSquare(board::toInt(board::Square::E8)), board::toInt(board::Piece::BR));

    int from = board::toInt(board::Square::E7);
    int to = board::toInt(board::Square::E8);
    int moveVal = board::Move::MOVE(from, to, board::toInt(board::Piece::BR), board::toInt(board::Piece::WQ), 0);
    board::Move move(moveVal, 0);

    board::makemove::move(move, pos);

    EXPECT_EQ(pos.getSquare(board::toInt(board::Square::E7)), board::toInt(board::Piece::EMPTY));
    EXPECT_EQ(pos.getSquare(board::toInt(board::Square::E8)), board::toInt(board::Piece::WQ));

    board::makemove::undo(pos);

    EXPECT_EQ(pos.getSquare(board::toInt(board::Square::E7)), board::toInt(board::Piece::WP));
    EXPECT_EQ(pos.getSquare(board::toInt(board::Square::E8)), board::toInt(board::Piece::BR));
}
