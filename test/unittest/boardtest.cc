#include "boardtest.h"

#include "gtest/gtest_pred_impl.h"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <utility>

using namespace test;

void BoardTest::ASSERT_BOARD(const board::Board& b, std::map<board::Square, board::Piece> pieces, board::Color side,
                             board::Square enPas, int fiftyMove, int castlePerm)
{
    ASSERT_EQ(b.getSide(), side);
    ASSERT_EQ(b.getEnPas(), enPas);
    ASSERT_EQ(b.getFiftyMove(), fiftyMove);
    ASSERT_EQ(b.getCastlePerm(), castlePerm);

    for (auto const& p : pieces) {
        ASSERT_EQ(b.getSquare(p.first), p.second);
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

TEST_F(BoardTest, sqAttackedPawn)
        {
            // White pawn attacks
            pos.parseFen("8/8/8/8/4p3/3P4/8/8 w - - 0 1");
            ASSERT_TRUE(pos.sqAttacked(board::C4, board::WHITE));  // d3 attacks c4
            ASSERT_TRUE(pos.sqAttacked(board::E4, board::WHITE));  // d3 attacks e4
            ASSERT_FALSE(pos.sqAttacked(board::D4, board::WHITE)); // d3 does not attack d4

            ASSERT_TRUE(pos.sqAttacked(board::D3, board::BLACK));  // e4 attacks d3
            ASSERT_TRUE(pos.sqAttacked(board::F3, board::BLACK));  // e4 attacks f3
            ASSERT_FALSE(pos.sqAttacked(board::E3, board::BLACK)); // e4 does not attack e3
        }

        TEST_F(BoardTest, sqAttackedKnight)
        {
            pos.parseFen("8/8/8/8/4N3/8/8/8 w - - 0 1");
            ASSERT_TRUE(pos.sqAttacked(board::C3, board::WHITE));
            ASSERT_TRUE(pos.sqAttacked(board::C5, board::WHITE));
            ASSERT_TRUE(pos.sqAttacked(board::D2, board::WHITE));
            ASSERT_TRUE(pos.sqAttacked(board::D6, board::WHITE));
            ASSERT_TRUE(pos.sqAttacked(board::F2, board::WHITE));
            ASSERT_TRUE(pos.sqAttacked(board::F6, board::WHITE));
            ASSERT_TRUE(pos.sqAttacked(board::G3, board::WHITE));
            ASSERT_TRUE(pos.sqAttacked(board::G5, board::WHITE));

            ASSERT_FALSE(pos.sqAttacked(board::E4, board::WHITE)); // Self square
            ASSERT_FALSE(pos.sqAttacked(board::E5, board::WHITE));
        }

        TEST_F(BoardTest, sqAttackedRook)
        {
            pos.parseFen("8/8/8/8/4R3/8/8/8 w - - 0 1");
            // Attacks on rank 4
            for (int f = board::FILE_A; f <= board::FILE_H; ++f) {
                if (f != board::FILE_E) {
                    ASSERT_TRUE(pos.sqAttacked(board::Board::FR2SQ(f, board::RANK_4), board::WHITE));
                }
            }
            // Attacks on file E
            for (int r = board::RANK_1; r <= board::RANK_8; ++r) {
                if (r != board::RANK_4) {
                    ASSERT_TRUE(pos.sqAttacked(board::Board::FR2SQ(board::FILE_E, r), board::WHITE));
                }
            }

            // Blocked by piece
            pos.parseFen("8/8/8/8/3PR3/8/8/8 w - - 0 1");
            ASSERT_TRUE(pos.sqAttacked(board::D4, board::WHITE));
            // C4 should NOT be attacked by Rook because Pawn at D4 blocks it
            // Wait, D4 is attacked. C4 is behind D4.
            // sqAttacked checks if a square is attacked by a side.
            // So if I ask "is C4 attacked by White?", it scans from C4 specifically for sliding pieces.
            // If it scans Right from C4, it hits D4 (White Pawn).
            // Does it continue?
            // The code:
            /*
            for (int i = 0; i < 4; i++) {
                targetSquare = square + MOVE_DIR[WR][i];
                piece = board[targetSquare];
                while (piece != NO_SQ) {
                    if (piece != EMPTY) {
                        if (side_in == WHITE && (piece == WR || piece == WQ)) {
                            return true;
                        }
                        ...
                        break;
                    }
                    ...
                }
            }
            */
            // If we check sqAttacked(C4, WHITE).
            // We look East. We find D4 (WP). It is not WR/WQ. Break.
            // So C4 is NOT attacked by WR.
            // Correct.

            ASSERT_FALSE(pos.sqAttacked(board::C4, board::WHITE));
        }

        TEST_F(BoardTest, sqAttackedBishop)
        {
            pos.parseFen("8/8/8/8/4B3/8/8/8 w - - 0 1");
            ASSERT_TRUE(pos.sqAttacked(board::F5, board::WHITE));
            ASSERT_TRUE(pos.sqAttacked(board::G6, board::WHITE));
            ASSERT_TRUE(pos.sqAttacked(board::H7, board::WHITE));

            ASSERT_TRUE(pos.sqAttacked(board::D3, board::WHITE));
            ASSERT_TRUE(pos.sqAttacked(board::C2, board::WHITE));
            ASSERT_TRUE(pos.sqAttacked(board::B1, board::WHITE));

            ASSERT_TRUE(pos.sqAttacked(board::D5, board::WHITE));
            ASSERT_TRUE(pos.sqAttacked(board::C6, board::WHITE));

            ASSERT_TRUE(pos.sqAttacked(board::F3, board::WHITE));
        }

        TEST_F(BoardTest, sqAttackedQueen)
        {
            // Queen combines Rook and Bishop. Not testing fully, but basic check.
            pos.parseFen("8/8/8/8/4Q3/8/8/8 w - - 0 1");
            ASSERT_TRUE(pos.sqAttacked(board::E8, board::WHITE));
            ASSERT_TRUE(pos.sqAttacked(board::A8, board::WHITE));
        }

        TEST_F(BoardTest, sqAttackedKing)
        {
            pos.parseFen("8/8/8/8/4K3/8/8/8 w - - 0 1");
            ASSERT_TRUE(pos.sqAttacked(board::D3, board::WHITE));
            ASSERT_TRUE(pos.sqAttacked(board::D4, board::WHITE));
            ASSERT_TRUE(pos.sqAttacked(board::D5, board::WHITE));
            ASSERT_TRUE(pos.sqAttacked(board::E3, board::WHITE));
            ASSERT_TRUE(pos.sqAttacked(board::E5, board::WHITE));
            ASSERT_TRUE(pos.sqAttacked(board::F3, board::WHITE));
            ASSERT_TRUE(pos.sqAttacked(board::F4, board::WHITE));
            ASSERT_TRUE(pos.sqAttacked(board::F5, board::WHITE));

            ASSERT_FALSE(pos.sqAttacked(board::E6, board::WHITE)); // Too far
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