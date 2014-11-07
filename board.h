#ifndef BOARD_H
#define BOARD_H

#include "bitboard.h"
#include "undo.h"
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

// Get the square from a file and rank
#define FR2SQ(file, rank) ((21 + file) + (rank * 10))

// Colors
enum { WHITE, BLACK, BOTH };

// Pieces
enum { EMPTY, WP, WN, WB, WR, WQ, WK, BP, BN, BB, BR, BQ, BK };

// Ranks
enum { RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8, RANK_NONE };

// Files
enum { FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_NONE };

// Castling permissions
enum { WKCA = 1, WQCA = 2, BKCA = 4, BQCA = 8 };

// Squares
enum {
    A1 = 21, B1, C1, D1, E1, F1, G1, H1,
    A2 = 31, B2, C2, D2, E2, F2, G2, H2,
    A3 = 41, B3, C3, D3, E3, F3, G3, H3,
    A4 = 51, B4, C4, D4, E4, F4, G4, H4,
    A5 = 61, B5, C5, D5, E5, F5, G5, H5,
    A6 = 71, B6, C6, D6, E6, F6, G6, H6,
    A7 = 81, B7, C7, D7, E7, F7, G7, H7,
    A8 = 91, B8, C8, D8, E8, F8, G8, H8, NO_SQ
};

// Piece colors
const int PIECE_COLOR[13] = { BOTH, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK };

// Piece values
const int PIECE_VAL[13] = { 0, 100, 300, 325, 500, 900, 0, 100, 300, 325, 500, 900, 0 };

// Piece is pawn
const bool PIECE_PAWN[13] = { false, true, false, false, false, false, false, true, false, false, false, false, false };

// Piece is knight
const bool PIECE_KNIGHT[13] = { false, false, true, false, false, false, false, false, true, false, false, false, false };

// Piece is bishop
const bool PIECE_BISHOP[13] = { false, false, false, true, false, false, false, false, false, true, false, false, false };

// Piece is rook
const bool PIECE_ROOK[13] = { false, false, false, false, true, false, false, false, false, false, true, false, false };

// Piece is queen
const bool PIECE_QUEEN[13] = { false, false, false, false, false, true, false, false, false, false, false, true, false };

// Piece is king
const bool PIECE_KING[13] = { false, false, false, false, false, false, true, false, false, false, false, false, true };

// Starting FEN
const std::string DEFAULT_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

// Piece chars
const std::string PIECE_CHARS = ".PNBRQKpnbrqk";

// Ranks
const int RANKS[64] = {
    RANK_1, RANK_1, RANK_1, RANK_1, RANK_1, RANK_1, RANK_1, RANK_1,
    RANK_2, RANK_2, RANK_2, RANK_2, RANK_2, RANK_2, RANK_2, RANK_2,
    RANK_3, RANK_3, RANK_3, RANK_3, RANK_3, RANK_3, RANK_3, RANK_3,
    RANK_4, RANK_4, RANK_4, RANK_4, RANK_4, RANK_4, RANK_4, RANK_4,
    RANK_5, RANK_5, RANK_5, RANK_5, RANK_5, RANK_5, RANK_5, RANK_5,
    RANK_6, RANK_6, RANK_6, RANK_6, RANK_6, RANK_6, RANK_6, RANK_6,
    RANK_7, RANK_7, RANK_7, RANK_7, RANK_7, RANK_7, RANK_7, RANK_7,
    RANK_8, RANK_8, RANK_8, RANK_8, RANK_8, RANK_8, RANK_8, RANK_8
};

// Files
const int FILES[64] = {
    FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H,
    FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H,
    FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H,
    FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H,
    FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H,
    FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H,
    FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H,
    FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H
};

// Convert from a 120 to 64 board index
const int SQ64[120] = {
    99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
    99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
    99,  0,  1,  2,  3,  4,  5,  6,  7, 99,
    99,  8,  9, 10, 11, 12, 13, 14, 15, 99,
    99, 16, 17, 18, 19, 20, 21, 22, 23, 99,
    99, 24, 25, 26, 27, 28, 29, 30, 31, 99,
    99, 32, 33, 34, 35, 36, 37, 38, 39, 99,
    99, 40, 41, 42, 43, 44, 45, 46, 47, 99,
    99, 48, 49, 50, 51, 52, 53, 54, 55, 99,
    99, 56, 57, 58, 59, 60, 61, 62, 63, 99,
    99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
    99, 99, 99, 99, 99, 99, 99, 99, 99, 99
};

// Convert from a 64 to 120 board index
const int SQ120[120] = {
    21, 22, 23, 24, 25, 26, 27, 28,
    31, 32, 33, 34, 35, 36, 37, 38,
    41, 42, 43, 44, 45, 46, 47, 48,
    51, 52, 53, 54, 55, 56, 57, 58,
    61, 62, 63, 64, 65, 66, 67, 68,
    71, 72, 73, 74, 75, 76, 77, 78,
    81, 82, 83, 84, 85, 86, 87, 88,
    91, 92, 93, 94, 95, 96, 97, 98
};

// Castle perm
const int  CASTLE_PERM[120] = {
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 13, 15, 15, 15, 12, 15, 15, 14, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15,  7, 15, 15, 15,  3, 15, 15, 11, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15
};

// Move directions
const int MOVE_DIR[13][8] = {
    {  0,   0,   0,   0,  0,   0,  0,  0 },
    {  0,   0,   0,   0,  0,   0,  0,  0 },
    { -8, -19, -21, -12,  8,  19, 21, 12 },
    { -9, -11,  11,   9,  0,   0,  0,  0 },
    { -1, -10,   1,  10,  0,   0,  0,  0 },
    { -9, -11,  11,   9, -1, -10,  1, 10 },
    { -9, -11,  11,   9, -1, -10,  1, 10 },
    {  0,   0,   0,   0,  0,   0,  0,  0 },
    { -8, -19, -21, -12,  8,  19, 21, 12 },
    { -9, -11,  11,   9,  0,   0,  0,  0 },
    { -1, -10,   1,  10,  0,   0,  0,  0 },
    { -9, -11,  11,   9, -1, -10,  1, 10 },
    { -9, -11,  11,   9, -1, -10,  1, 10 }
};

// Zobrist piece hash keys
extern uint64_t PIECE_KEYS[13][120];

// Zobrist side hash key
extern uint64_t CASTLE_KEYS[16];

// Zobrist castle hash keys
extern uint64_t SIDE_KEY;

// Board representation
class Board
{
    private:

        // Board array
        int board[120];

        // Side to move
        int side;

        // Current ply
        int ply;

        // Fifty move rule count
        int fiftyMove;

        // En passant square
        int enPas;

        // Castling permutation
        int castlePerm;

        // Position key
        uint64_t hashKey;

        // Pawn bitboard
        uint64_t pawns[3];

        // Piece list
        int pList[13][10];

        // Piece list counter
        int pNum[13];

        // Material count
        int material[2];

        // Game history
        std::vector<Undo> history;

        // Moves that beat alpha
        int searchHistory[13][120];

        // Beta cutoffs
        std::vector<int> searchKillers[2];

    public:

        // Create a board with the standard starting position
        Board() { parseFen(DEFAULT_FEN); };

        // Create a board withe a given position
        Board(const std::string fen) { parseFen(fen); };

        // Generate a unique position key
        void generateHash();

        // Reset the board
        void reset();

        // Parse a FEN string
        bool parseFen(const std::string);

        // Print the board
        void print() const;

        // Update piece list and material
        void updateListMaterial();

        // Check if a square is attacked by a side
        bool sqAttacked(const int, const int) const;

        // Update the castle perm
        void updateCastlePerm(const int, const int);

        // Clear search history
        void clearSearchHistory();

        // Clear search killers
        void clearSearchKillers();

        // Add to the history
        void addHistory(Undo& undo);

        // Remove history
        Undo removeHistory();

        // Switch side
        void updateSide() { side ^= 1; };

        // Hash a piece key
        void hashPiece(const int piece, const int square) { hashKey ^= PIECE_KEYS[piece][square]; };

        // Hash a castle key
        void hashCastle() { hashKey ^= CASTLE_KEYS[castlePerm]; };

        // Hash the En Pasant key
        void hashEnPas() { hashKey ^= PIECE_KEYS[EMPTY][enPas]; };

        // Hash the side key
        void hashSide() { hashKey ^= SIDE_KEY; };

        // Increment the piece num
        void incrementPieceNum(const int piece) { pNum[piece]++; };

        // Decrement the piece num
        void decrementPieceNum(const int piece) { pNum[piece]--; };

        // Increment fifty move counter
        void incrementFiftyMove() { fiftyMove++; };

        // Reset fifty move counter
        void resetFiftyMove() { fiftyMove = 0; };

        // Increment ply
        void incrementPly() { ply++; };

        // Decrement ply
        void decrementPly() { ply--; };

        // Reset ply
        void resetPly() { ply =0; };

        // Clear En pasant square
        void clearEnPas() { enPas = NO_SQ; };

        // Add to material count
        void addMaterial(const int side, const int val) { material[side] += val; };

        // Get a square
        int getSquare(const int square) const { return board[square]; };

        // Set a square
        void setSquare(const int square, const int value) { board[square] = value; };

        // Get the side to move
        int getSide() const { return side; };

        // Get the ply
        int getPly() const { return ply; };

        // Get fifty move counter
        int getFiftyMove() const { return fiftyMove; };

        // Set the fifty move count
        void setFiftyMove(const int count) { fiftyMove = count; };

        // Get En pasant square
        int getEnPas() const { return enPas; };

        // Set En passant square
        void setEnPas(const int square) { enPas = square; };

        // Get hash key
        uint64_t getHashKey() const { return hashKey; };

        // Get castle perm
        int getCastlePerm() const { return castlePerm; };

        // Set castle perm
        void setCastlePerm(const int perm) { castlePerm = perm; };

        // Get pawn bitboard
        uint64_t* getPawns() { return pawns; };

        // Get the piece list
        int* getPieceList(const int piece) { return pList[piece]; };

        // Get the piece list counter
        int getPieceNum(const int piece) const { return pNum[piece]; };

        // Get the history
        std::vector<Undo> getHistory() { return history; };

        // Get material for a side
        int getMaterial(const int side) const { return material[side]; }
};

#endif
