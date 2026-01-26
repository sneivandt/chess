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

// Test for Bug Fix: Piece list overflow should cause parseFen to fail
TEST_F(BoardTest, PieceListBoundsOverflow)
{
    // Create a FEN with too many pieces of one type (e.g., 11+ queens via promotions)
    // This is an illegal position but tests the bounds check
    // Note: pList array is [13][10], so it can hold indices 0-9 (10 pieces max)
    
    // 10 white queens - should work (at the limit with king)
    std::string fen_10q = "QQQQQQQQ/QQK5/8/8/8/8/8/8 w - - 0 1";
    ASSERT_TRUE(pos.parseFen(fen_10q));
    ASSERT_EQ(pos.getPieceNum(board::WQ), 10);
    
    // 11 white queens - should fail due to bounds check (exceeds array capacity)
    // The updateListMaterial function will return false when overflow detected
    std::string fen_11q = "QQQQQQQQ/QQQK4/8/8/8/8/8/8 w - - 0 1";
    ASSERT_FALSE(pos.parseFen(fen_11q)); // Should return false for invalid position
}

// Test for Bug Fix 3: FEN parsing integer overflow in fiftyMove
TEST_F(BoardTest, ParseFenLargeFiftyMove)
{
    // Test with a very large fiftyMove value that would cause integer overflow
    // Should be capped at 1000
    std::string fen_overflow = "8/8/8/8/8/8/8/8 w - - 99999999999999 1";
    ASSERT_TRUE(pos.parseFen(fen_overflow));
    
    // Should be capped at 1000, not overflowed
    ASSERT_EQ(pos.getFiftyMove(), 1000);
    
    // Test with a reasonable large value
    std::string fen_large = "8/8/8/8/8/8/8/8 w - - 999 1";
    ASSERT_TRUE(pos.parseFen(fen_large));
    ASSERT_EQ(pos.getFiftyMove(), 999);
    
    // Test with exactly 1000
    std::string fen_1000 = "8/8/8/8/8/8/8/8 w - - 1000 1";
    ASSERT_TRUE(pos.parseFen(fen_1000));
    ASSERT_EQ(pos.getFiftyMove(), 1000);
    
    // Test with value just above 1000 - should be capped
    std::string fen_1001 = "8/8/8/8/8/8/8/8 w - - 1001 1";
    ASSERT_TRUE(pos.parseFen(fen_1001));
    ASSERT_EQ(pos.getFiftyMove(), 1000);
    
    // Test with value moderately above 1000 - should be capped
    std::string fen_2000 = "8/8/8/8/8/8/8/8 w - - 2000 1";
    ASSERT_TRUE(pos.parseFen(fen_2000));
    ASSERT_EQ(pos.getFiftyMove(), 1000);
}

// Test for Bug Fix 2: FEN parsing en passant with correct character consumption
TEST_F(BoardTest, ParseFenEnPassantParsing)
{
    // Test with en passant square (e3)
    std::string fen_with_ep = "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1";
    ASSERT_TRUE(pos.parseFen(fen_with_ep));
    ASSERT_EQ(pos.getEnPas(), board::E3);
    ASSERT_EQ(pos.getFiftyMove(), 0); // Should correctly parse fifty-move after en passant
    
    // Test with en passant square (d6)
    std::string fen_with_ep2 = "rnbqkbnr/ppp1pppp/8/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 2";
    ASSERT_TRUE(pos.parseFen(fen_with_ep2));
    ASSERT_EQ(pos.getEnPas(), board::D6);
    ASSERT_EQ(pos.getFiftyMove(), 0);
    
    // Test with no en passant (dash)
    std::string fen_no_ep = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 5 1";
    ASSERT_TRUE(pos.parseFen(fen_no_ep));
    ASSERT_EQ(pos.getEnPas(), board::NO_SQ);
    ASSERT_EQ(pos.getFiftyMove(), 5); // Should correctly parse fifty-move after dash
    
    // Test with various en passant squares and different fifty-move values
    std::string fen_ep_a6 = "8/8/8/8/8/8/8/8 w KQkq a6 42 10";
    ASSERT_TRUE(pos.parseFen(fen_ep_a6));
    ASSERT_EQ(pos.getEnPas(), board::A6);
    ASSERT_EQ(pos.getFiftyMove(), 42);
}