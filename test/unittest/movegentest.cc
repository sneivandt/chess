#include "gtest/gtest.h"
#include "board/board.h"
#include "board/makemove.h"
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

TEST_F(MoveGenTest, GenerateMultiplePawnCaptures)
{
    // Multiple captures available
    pos.parseFen("rnbqkbnr/ppp2ppp/8/3pp3/3PP3/8/PPP2PPP/RNBQKBNR w KQkq - 0 3");

    search::MoveList captures = search::movegen::generateAll(pos, true);
    EXPECT_EQ(captures.getMoves().size(), 2); // dxe5 and exd5
}

TEST_F(MoveGenTest, GenerateKnightMoves)
{
    // Knight in center should have 8 moves, plus 5 king moves = 13 total
    pos.parseFen("4k3/8/8/4N3/8/8/8/4K3 w - - 0 1");

    search::MoveList moves = search::movegen::generateAll(pos, false);
    EXPECT_EQ(moves.getMoves().size(), 13);
}

TEST_F(MoveGenTest, GenerateCastlingMoves)
{
    // White can castle both sides
    pos.parseFen("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");

    search::MoveList moves = search::movegen::generateAll(pos, false);
    
    // Count castling moves
    int castlingMoves = 0;
    for (const auto& move : moves.getMoves()) {
        if (move.getValue() & board::Move::MFLAGCA) {
            castlingMoves++;
        }
    }
    EXPECT_EQ(castlingMoves, 2); // Kingside and queenside
}

TEST_F(MoveGenTest, GenerateEnPassantCapture)
{
    // En passant available
    pos.parseFen("rnbqkbnr/ppp2ppp/8/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 3");

    search::MoveList captures = search::movegen::generateAll(pos, true);
    
    // Check for en passant move
    bool foundEnPassant = false;
    for (const auto& move : captures.getMoves()) {
        if (move.getValue() & board::Move::MFLAGEP) {
            foundEnPassant = true;
            EXPECT_EQ(board::Move::FROMSQ(move.getValue()), board::E5);
            EXPECT_EQ(board::Move::TOSQ(move.getValue()), board::D6);
        }
    }
    EXPECT_TRUE(foundEnPassant);
}

TEST_F(MoveGenTest, GeneratePromotionMoves)
{
    // White pawn can promote
    pos.parseFen("8/P7/8/8/8/8/8/4K2k w - - 0 1");

    search::MoveList moves = search::movegen::generateAll(pos, false);
    
    // Should have 4 promotion moves (Q, R, B, N)
    int promotionMoves = 0;
    for (const auto& move : moves.getMoves()) {
        int promoted = board::Move::PROMOTED(move.getValue());
        if (promoted != board::EMPTY) {
            promotionMoves++;
        }
    }
    EXPECT_EQ(promotionMoves, 4);
}

TEST_F(MoveGenTest, GenerateNoMovesInCheckmate)
{
    // Fool's mate position
    pos.parseFen("rnb1kbnr/pppp1ppp/8/4p3/6Pq/5P2/PPPPP2P/RNBQKBNR w KQkq - 1 3");

    auto moves = search::movegen::generateAll(pos, false).getMoves();
    
    // Check that no legal moves (all moves leave king in check)
    int legalMoves = 0;
    for (auto move : moves) {
        if (board::makemove::move(move, pos)) {
            legalMoves++;
            board::makemove::undo(pos);
        }
    }
    EXPECT_EQ(legalMoves, 0);
}

TEST_F(MoveGenTest, GeneratePawnDoubleMoveFromStartingRank)
{
    // Test that white pawns can double-move from rank 2 (their starting rank)
    pos.parseFen(board::Board::DEFAULT_FEN);
    
    search::MoveList moves = search::movegen::generateAll(pos, false);
    
    // Count pawn double-moves for white (should be 8 - one for each pawn)
    int doubleMoves = 0;
    for (const auto& move : moves.getMoves()) {
        if (move.getValue() & board::Move::MFLAGPS) { // MFLAGPS = pawn start (double move)
            doubleMoves++;
            // Verify these are from rank 2 to rank 4
            int from = board::Move::FROMSQ(move.getValue());
            int to = board::Move::TOSQ(move.getValue());
            EXPECT_EQ(board::Board::RANKS[board::Board::SQ64[from]], board::RANK_2);
            EXPECT_EQ(board::Board::RANKS[board::Board::SQ64[to]], board::RANK_4);
        }
    }
    EXPECT_EQ(doubleMoves, 8);
    
    // Test for black pawns from rank 7
    pos.parseFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1");
    moves = search::movegen::generateAll(pos, false);
    
    doubleMoves = 0;
    for (const auto& move : moves.getMoves()) {
        if (move.getValue() & board::Move::MFLAGPS) {
            doubleMoves++;
            // Verify these are from rank 7 to rank 5
            int from = board::Move::FROMSQ(move.getValue());
            int to = board::Move::TOSQ(move.getValue());
            EXPECT_EQ(board::Board::RANKS[board::Board::SQ64[from]], board::RANK_7);
            EXPECT_EQ(board::Board::RANKS[board::Board::SQ64[to]], board::RANK_5);
        }
    }
    EXPECT_EQ(doubleMoves, 8);
}
