#include "boardtest.h"

#include "search/searchstate.h"

#include "gtest/gtest_pred_impl.h"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <utility>

using namespace test;

void BoardTest::ASSERT_BOARD(const board::Board& b, std::map<board::Square, board::Piece> pieces, board::Color side,
                             board::Square enPas, int fiftyMove, int castlePerm)
{
    ASSERT_EQ(b.getSide(), board::toInt(side));
    ASSERT_EQ(b.getEnPas(), board::toInt(enPas));
    ASSERT_EQ(b.getFiftyMove(), fiftyMove);
    ASSERT_EQ(b.getCastlePerm(), castlePerm);

    for (auto const& p : pieces) {
        ASSERT_EQ(b.getSquare(board::toInt(p.first)), board::toInt(p.second));
    }
}

TEST_F(BoardTest, parseFenInvalid)
{
    ASSERT_FALSE(pos.parseFen("foo"));
}

TEST_F(BoardTest, parseFenTooManyFilesInRank)
{
    // Test FEN with 9 pieces on first rank (should be rejected)
    ASSERT_FALSE(pos.parseFen("rnbqkbnrr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"));
}

TEST_F(BoardTest, parseFenInvalidEnPassantFile)
{
    // Test FEN with invalid en passant file 'z'
    ASSERT_FALSE(pos.parseFen("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq z3 0 1"));
}

TEST_F(BoardTest, parseFenInvalidEnPassantRank)
{
    // Test FEN with invalid en passant rank '9'
    ASSERT_FALSE(pos.parseFen("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e9 0 1"));
}

TEST_F(BoardTest, parseFenInvalidEnPassantRank0)
{
    // Test FEN with invalid en passant rank '0'
    ASSERT_FALSE(pos.parseFen("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e0 0 1"));
}

TEST_F(BoardTest, parseFenRejectsMalformedFields)
{
    ASSERT_FALSE(pos.parseFen("8/8/8/8/8/8/8/8 x - - 0 1"));
    ASSERT_FALSE(pos.parseFen("8/8/8/8/8/8/8/8 w KK - 0 1"));
    ASSERT_FALSE(pos.parseFen("8/8/8/8/8/8/8/8 w KX - 0 1"));
    ASSERT_FALSE(pos.parseFen("8/8/8/8/8/8/8/8 w - e4 0 1"));
    ASSERT_FALSE(pos.parseFen("8/8/8/8/8/8/8/8 w - - -1 1"));
    ASSERT_FALSE(pos.parseFen("8/8/8/8/8/8/8/8 w - - 0 0"));
    ASSERT_FALSE(pos.parseFen("8/8/8/8/8/8/8 w - - 0 1"));
}

TEST_F(BoardTest, parseFenStartPos)
{
    ASSERT_TRUE(pos.parseFen(board::Board::DEFAULT_FEN));

    ASSERT_BOARD(pos,
                 {

                 },
                 board::Color::WHITE, board::Square::NO_SQ, 0);
}

TEST_F(BoardTest, HashKeyIncludesCastlingRights)
{
    ASSERT_TRUE(pos.parseFen("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1"));
    const uint64_t allCastlingRightsHash = pos.getHashKey();

    ASSERT_TRUE(pos.parseFen("r3k2r/8/8/8/8/8/8/R3K2R w - - 0 1"));
    EXPECT_NE(pos.getHashKey(), allCastlingRightsHash);
}

TEST_F(BoardTest, HashKeyInitializationIsDeterministic)
{
    ASSERT_TRUE(pos.parseFen(board::Board::DEFAULT_FEN));
    const uint64_t firstHash = pos.getHashKey();

    board::Board::INIT();
    ASSERT_TRUE(pos.parseFen(board::Board::DEFAULT_FEN));
    EXPECT_EQ(pos.getHashKey(), firstHash);
}

TEST_F(BoardTest, sqAttackedPawn)
{
    // White pawn attacks
    pos.parseFen("8/8/8/8/4p3/3P4/8/8 w - - 0 1");
    ASSERT_TRUE(pos.sqAttacked(board::toInt(board::Square::C4), board::toInt(board::Color::WHITE))); // d3 attacks c4
    ASSERT_TRUE(pos.sqAttacked(board::toInt(board::Square::E4), board::toInt(board::Color::WHITE))); // d3 attacks e4
    ASSERT_FALSE(
        pos.sqAttacked(board::toInt(board::Square::D4), board::toInt(board::Color::WHITE))); // d3 does not attack d4

    ASSERT_TRUE(pos.sqAttacked(board::toInt(board::Square::D3), board::toInt(board::Color::BLACK))); // e4 attacks d3
    ASSERT_TRUE(pos.sqAttacked(board::toInt(board::Square::F3), board::toInt(board::Color::BLACK))); // e4 attacks f3
    ASSERT_FALSE(
        pos.sqAttacked(board::toInt(board::Square::E3), board::toInt(board::Color::BLACK))); // e4 does not attack e3
}

TEST_F(BoardTest, sqAttackedKnight)
{
    pos.parseFen("8/8/8/8/4N3/8/8/8 w - - 0 1");
    ASSERT_TRUE(pos.sqAttacked(board::toInt(board::Square::C3), board::toInt(board::Color::WHITE)));
    ASSERT_TRUE(pos.sqAttacked(board::toInt(board::Square::C5), board::toInt(board::Color::WHITE)));
    ASSERT_TRUE(pos.sqAttacked(board::toInt(board::Square::D2), board::toInt(board::Color::WHITE)));
    ASSERT_TRUE(pos.sqAttacked(board::toInt(board::Square::D6), board::toInt(board::Color::WHITE)));
    ASSERT_TRUE(pos.sqAttacked(board::toInt(board::Square::F2), board::toInt(board::Color::WHITE)));
    ASSERT_TRUE(pos.sqAttacked(board::toInt(board::Square::F6), board::toInt(board::Color::WHITE)));
    ASSERT_TRUE(pos.sqAttacked(board::toInt(board::Square::G3), board::toInt(board::Color::WHITE)));
    ASSERT_TRUE(pos.sqAttacked(board::toInt(board::Square::G5), board::toInt(board::Color::WHITE)));

    ASSERT_FALSE(pos.sqAttacked(board::toInt(board::Square::E4), board::toInt(board::Color::WHITE))); // Self square
    ASSERT_FALSE(pos.sqAttacked(board::toInt(board::Square::E5), board::toInt(board::Color::WHITE)));
}

TEST_F(BoardTest, sqAttackedRook)
{
    pos.parseFen("8/8/8/8/4R3/8/8/8 w - - 0 1");
    // Attacks on rank 4
    for (int f = board::toInt(board::File::FILE_A); f <= board::toInt(board::File::FILE_H); ++f) {
        if (f != board::toInt(board::File::FILE_E)) {
            ASSERT_TRUE(pos.sqAttacked(board::Board::FR2SQ(f, board::toInt(board::Rank::RANK_4)),
                                       board::toInt(board::Color::WHITE)));
        }
    }
    // Attacks on file E
    for (int r = board::toInt(board::Rank::RANK_1); r <= board::toInt(board::Rank::RANK_8); ++r) {
        if (r != board::toInt(board::Rank::RANK_4)) {
            ASSERT_TRUE(pos.sqAttacked(board::Board::FR2SQ(board::toInt(board::File::FILE_E), r),
                                       board::toInt(board::Color::WHITE)));
        }
    }

    // Blocked by piece
    pos.parseFen("8/8/8/8/3PR3/8/8/8 w - - 0 1");
    ASSERT_TRUE(pos.sqAttacked(board::toInt(board::Square::D4), board::toInt(board::Color::WHITE)));
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

    ASSERT_FALSE(pos.sqAttacked(board::toInt(board::Square::C4), board::toInt(board::Color::WHITE)));
}

TEST_F(BoardTest, sqAttackedBishop)
{
    pos.parseFen("8/8/8/8/4B3/8/8/8 w - - 0 1");
    ASSERT_TRUE(pos.sqAttacked(board::toInt(board::Square::F5), board::toInt(board::Color::WHITE)));
    ASSERT_TRUE(pos.sqAttacked(board::toInt(board::Square::G6), board::toInt(board::Color::WHITE)));
    ASSERT_TRUE(pos.sqAttacked(board::toInt(board::Square::H7), board::toInt(board::Color::WHITE)));

    ASSERT_TRUE(pos.sqAttacked(board::toInt(board::Square::D3), board::toInt(board::Color::WHITE)));
    ASSERT_TRUE(pos.sqAttacked(board::toInt(board::Square::C2), board::toInt(board::Color::WHITE)));
    ASSERT_TRUE(pos.sqAttacked(board::toInt(board::Square::B1), board::toInt(board::Color::WHITE)));

    ASSERT_TRUE(pos.sqAttacked(board::toInt(board::Square::D5), board::toInt(board::Color::WHITE)));
    ASSERT_TRUE(pos.sqAttacked(board::toInt(board::Square::C6), board::toInt(board::Color::WHITE)));

    ASSERT_TRUE(pos.sqAttacked(board::toInt(board::Square::F3), board::toInt(board::Color::WHITE)));
}

TEST_F(BoardTest, sqAttackedQueen)
{
    // Queen combines Rook and Bishop. Not testing fully, but basic check.
    pos.parseFen("8/8/8/8/4Q3/8/8/8 w - - 0 1");
    ASSERT_TRUE(pos.sqAttacked(board::toInt(board::Square::E8), board::toInt(board::Color::WHITE)));
    ASSERT_TRUE(pos.sqAttacked(board::toInt(board::Square::A8), board::toInt(board::Color::WHITE)));
}

TEST_F(BoardTest, sqAttackedKing)
{
    pos.parseFen("8/8/8/8/4K3/8/8/8 w - - 0 1");
    ASSERT_TRUE(pos.sqAttacked(board::toInt(board::Square::D3), board::toInt(board::Color::WHITE)));
    ASSERT_TRUE(pos.sqAttacked(board::toInt(board::Square::D4), board::toInt(board::Color::WHITE)));
    ASSERT_TRUE(pos.sqAttacked(board::toInt(board::Square::D5), board::toInt(board::Color::WHITE)));
    ASSERT_TRUE(pos.sqAttacked(board::toInt(board::Square::E3), board::toInt(board::Color::WHITE)));
    ASSERT_TRUE(pos.sqAttacked(board::toInt(board::Square::E5), board::toInt(board::Color::WHITE)));
    ASSERT_TRUE(pos.sqAttacked(board::toInt(board::Square::F3), board::toInt(board::Color::WHITE)));
    ASSERT_TRUE(pos.sqAttacked(board::toInt(board::Square::F4), board::toInt(board::Color::WHITE)));
    ASSERT_TRUE(pos.sqAttacked(board::toInt(board::Square::F5), board::toInt(board::Color::WHITE)));

    ASSERT_FALSE(pos.sqAttacked(board::toInt(board::Square::E6), board::toInt(board::Color::WHITE))); // Too far
}

TEST_F(BoardTest, parseFenRuyLopez)
{
    ASSERT_TRUE(pos.parseFen("r1bqkbnr/pppp1ppp/2n5/1B2p3/4P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 3 3"));

    ASSERT_BOARD(pos,
                 {
                     {board::Square::E4, board::Piece::WP},
                     {board::Square::E5, board::Piece::BP},
                     {board::Square::F3, board::Piece::WN},
                     {board::Square::C6, board::Piece::BN},
                     {board::Square::B5, board::Piece::WB},
                 },
                 board::Color::BLACK, board::Square::NO_SQ, 3);
}

TEST_F(BoardTest, parseFenSicilianDefence)
{
    ASSERT_TRUE(pos.parseFen("rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 0 2"));

    ASSERT_BOARD(pos,
                 {
                     {board::Square::E4, board::Piece::WP},
                     {board::Square::C5, board::Piece::BP},
                 },
                 board::Color::WHITE, board::Square::NO_SQ, 0);
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
    ASSERT_EQ(pos.getPieceNum(board::toInt(board::Piece::WQ)), 10);

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
    ASSERT_EQ(pos.getEnPas(), board::toInt(board::Square::E3));
    ASSERT_EQ(pos.getFiftyMove(), 0); // Should correctly parse fifty-move after en passant

    // Test with en passant square (d6)
    std::string fen_with_ep2 = "rnbqkbnr/ppp1pppp/8/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 2";
    ASSERT_TRUE(pos.parseFen(fen_with_ep2));
    ASSERT_EQ(pos.getEnPas(), board::toInt(board::Square::D6));
    ASSERT_EQ(pos.getFiftyMove(), 0);

    // Test with no en passant (dash)
    std::string fen_no_ep = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 5 1";
    ASSERT_TRUE(pos.parseFen(fen_no_ep));
    ASSERT_EQ(pos.getEnPas(), board::toInt(board::Square::NO_SQ));
    ASSERT_EQ(pos.getFiftyMove(), 5); // Should correctly parse fifty-move after dash

    // Test with various en passant squares and different fifty-move values
    std::string fen_ep_a6 = "8/8/8/8/8/8/8/8 w KQkq a6 42 10";
    ASSERT_TRUE(pos.parseFen(fen_ep_a6));
    ASSERT_EQ(pos.getEnPas(), board::toInt(board::Square::A6));
    ASSERT_EQ(pos.getFiftyMove(), 42);
}

// Test bounds checking in incrementSearchHistory
TEST_F(BoardTest, IncrementSearchHistoryBoundsCheck)
{
    ASSERT_TRUE(pos.parseFen(board::Board::DEFAULT_FEN));

    search::SearchState ss;

    // Valid bounds - should work and modify search history
    ss.clear();
    ss.incrementHistory(board::toInt(board::Square::E2), 50, 10, pos.getSquare(board::toInt(board::Square::E2)));
    ASSERT_EQ(ss.getHistory(pos.getSquare(board::toInt(board::Square::E2)), 50), 10);

    ss.incrementHistory(board::toInt(board::Square::E2), 50, 5, pos.getSquare(board::toInt(board::Square::E2)));
    ASSERT_EQ(ss.getHistory(pos.getSquare(board::toInt(board::Square::E2)), 50), 15); // Should accumulate

    ss.incrementHistory(board::toInt(board::Square::E2), 0, 3, pos.getSquare(board::toInt(board::Square::E2)));
    ASSERT_EQ(ss.getHistory(pos.getSquare(board::toInt(board::Square::E2)), 0), 3);

    ss.incrementHistory(board::toInt(board::Square::E2), 119, 7, pos.getSquare(board::toInt(board::Square::E2)));
    ASSERT_EQ(ss.getHistory(pos.getSquare(board::toInt(board::Square::E2)), 119), 7);

    // Test with invalid 'to' values - should be silently ignored, values unchanged
    int prevValue = ss.getHistory(pos.getSquare(board::toInt(board::Square::E2)), 50);
    ss.incrementHistory(board::toInt(board::Square::E2), -1, 100, pos.getSquare(board::toInt(board::Square::E2)));
    ss.incrementHistory(board::toInt(board::Square::E2), 120, 100, pos.getSquare(board::toInt(board::Square::E2)));
    ss.incrementHistory(board::toInt(board::Square::E2), 200, 100, pos.getSquare(board::toInt(board::Square::E2)));
    ASSERT_EQ(ss.getHistory(pos.getSquare(board::toInt(board::Square::E2)), 50), prevValue); // Should remain unchanged

    // Test with invalid 'from' that would produce out-of-bounds piece index
    // These should also be handled gracefully without crashing
    ss.incrementHistory(121, 50, 10, 0);
    ss.incrementHistory(-1, 50, 10, 0);
}

// Test bounds checking in search killer moves
TEST_F(BoardTest, SearchKillersBoundsCheck)
{
    ASSERT_TRUE(pos.parseFen(board::Board::DEFAULT_FEN));

    search::SearchState ss;

    // Set ply to valid value and store a killer move
    pos.resetPly();
    ss.addKiller(12345, pos.getPly());
    ASSERT_EQ(ss.getKiller(0, pos.getPly()), 12345);
    ASSERT_EQ(ss.getKiller(1, pos.getPly()), 0); // Second killer should be 0

    // Add another killer, first should move to second slot
    ss.addKiller(54321, pos.getPly());
    ASSERT_EQ(ss.getKiller(0, pos.getPly()), 54321);
    ASSERT_EQ(ss.getKiller(1, pos.getPly()), 12345);

    // Test with ply at boundary (127)
    for (int i = 0; i < 127; i++) {
        pos.incrementPly();
    }
    ss.addKiller(99999, pos.getPly());
    ASSERT_EQ(ss.getKiller(0, pos.getPly()), 99999);

    // Store another valid killer at a different ply level to test preservation
    pos.resetPly();
    pos.incrementPly(); // ply = 1
    ss.addKiller(77777, pos.getPly());
    ASSERT_EQ(ss.getKiller(0, pos.getPly()), 77777);

    // Test with ply beyond bounds - should be handled gracefully
    for (int i = 0; i < 128; i++) {
        pos.incrementPly();
    } // ply = 129, out of bounds
    ss.addKiller(11111, pos.getPly());           // Should not crash or corrupt data
    ASSERT_EQ(ss.getKiller(0, pos.getPly()), 0); // Should return 0 for out of bounds

    // Verify that previously stored valid killer moves are intact
    pos.resetPly();
    pos.incrementPly();                              // ply = 1
    ASSERT_EQ(ss.getKiller(0, pos.getPly()), 77777); // Should still be there, uncorrupted

    // Test with invalid num parameter
    pos.resetPly();
    ASSERT_EQ(ss.getKiller(-1, pos.getPly()), 0);
    ASSERT_EQ(ss.getKiller(2, pos.getPly()), 0);
    ASSERT_EQ(ss.getKiller(10, pos.getPly()), 0);
}
