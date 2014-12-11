#ifndef BOARD_H
#define BOARD_H

#include "bitboard.h"
#include "undo.h"
#include <cmath>
#include <cstdint>
#include <iostream>
#include <random>
#include <string>
#include <vector>

// Piece colors
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

// Board representation
class Board
{
    private:

        // Board array
        int board[120];

        // Side to move
        int side;

        // Search ply
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

        // Non capture beta cutoffs
        int searchKillers[2][64];

    public:

        Board() { parseFen(DEFAULT_FEN); };
        Board(const std::string fen) { parseFen(fen); };

        // Default position FEN
        static const std::string DEFAULT_FEN;

        // Piece chars
        static const std::string PIECE_CHARS;

        // Piece colors
        static const int PIECE_COLOR[13];

        // Piece without team
        static const int PIECE_NO_TEAM[13];

        // Piece values
        static const int PIECE_VAL[13];

        // Ranks
        static const int RANKS[64];

        // Files
        static const int FILES[64];

        // Convert from a 120 to 64 board index
        static const int SQ64[120];

        // Convert from a 64 to 120 board index
        static const int SQ120[64];

        // Castle perm mask
        static const int CASTLE_PERM_MASK[120];

        // Move directions
        static const int MOVE_DIR[13][8];

        // Zobrist hash keys
        static uint64_t PIECE_KEYS[13][120];
        static uint64_t CASTLE_KEYS[16];
        static uint64_t SIDE_KEY;

        // Bitmasks
        static uint64_t RANK_MASK[8];
        static uint64_t FILE_MASK[8];
        static uint64_t PASSED_PAWN_MASK[2][64];
        static uint64_t ISOLATED_PAWN_MASK[64];

        // Init
        static void INIT();

        // Get the square from a file and rank
        static int FR2SQ(const int, const int);

        // Reset the board
        void reset();

        // Parse a FEN string
        bool parseFen(const std::string);

        // Print the board
        void print() const;

        // Check if a square is attacked
        bool sqAttacked(const int, const int) const;

        // Update lists and material
        void updateListMaterial();

        // Board square
        int getSquare(const int) const;
        void setSquare(const int, const int);

        // Bitboards
        uint64_t* getPawns();

        // Piece lists
        void incrementPieceNum(const int);
        void decrementPieceNum(const int);
        int getPieceNum(const int) const;
        int* getPieceList(const int);

        // Side to move
        void updateSide();
        int getSide() const;

        // En pasant
        int getEnPas() const;
        void setEnPas(const int);
        void clearEnPas();

        // Castle perm
        int getCastlePerm() const;
        void setCastlePerm(const int);
        void updateCastlePerm(const int, const int);

        // Fifty move counter
        int getFiftyMove() const;
        void setFiftyMove(const int);
        void incrementFiftyMove();
        void resetFiftyMove();

        // Hash key
        void generateHash();
        uint64_t getHashKey() const;
        void hashSide();
        void hashPiece(const int, const int);
        void hashCastle();
        void hashEnPas();

        // Material count
        int getMaterial(const int) const;
        void addMaterial(const int, const int);

        // Game history
        std::vector<Undo> getHistory();
        void addHistory(Undo&);
        Undo popHistory();

        // Search ply
        int getPly() const;
        void incrementPly();
        void decrementPly();
        void resetPly();

        // Search history
        int getSearchKiller(const int) const;
        void addSearchKiller(const int);
        void clearSearchKillers();
        int getSearchHistory(const int, const int) const;
        void incrementSearchHistory(const int, const int, const int);
        void clearSearchHistory();
};

inline int Board::FR2SQ(const int file, const int rank)
{
    return (21 + file) + (rank * 10);
}

inline int Board::getSquare(const int square) const
{
    return board[square];
}

inline void Board::setSquare(const int square, const int value)
{
    board[square] = value;
}

inline uint64_t* Board::getPawns()
{
    return pawns;
}

inline void Board::incrementPieceNum(const int piece)
{
    pNum[piece]++;
}

inline void Board::decrementPieceNum(const int piece)
{
    pNum[piece]--;
}

inline int Board::getPieceNum(const int piece) const
{
    return pNum[piece];
}

inline int* Board::getPieceList(const int piece)
{
    return pList[piece];
}

inline void Board::updateSide()
{
    side ^= 1;
}

inline int Board::getSide() const
{
    return side;
}

inline int Board::getEnPas() const
{
    return enPas;
}

inline void Board::setEnPas(const int s)
{
    enPas = s;
}

inline void Board::clearEnPas()
{
    enPas = NO_SQ;
}

inline int Board::getCastlePerm() const
{
    return castlePerm;
}

inline void Board::setCastlePerm(const int p)
{
    castlePerm = p;
}

inline void Board::updateCastlePerm(const int to, const int from)
{
    castlePerm &= CASTLE_PERM_MASK[to] & CASTLE_PERM_MASK[from];
}

inline int Board::getFiftyMove() const
{
    return fiftyMove;
}

inline void Board::setFiftyMove(const int fm)
{
    fiftyMove = fm;
}

inline void Board::incrementFiftyMove()
{
    fiftyMove++;
}

inline void Board::resetFiftyMove()
{
    fiftyMove = 0;
}

inline uint64_t Board::getHashKey() const
{
    return hashKey;
}

inline void Board::hashSide()
{
    hashKey ^= SIDE_KEY;
}

inline void Board::hashPiece(const int piece, const int square)
{
    hashKey ^= PIECE_KEYS[piece][square];
}

inline void Board::hashCastle()
{
    hashKey ^= CASTLE_KEYS[castlePerm];
}

inline void Board::hashEnPas()
{
    hashKey ^= PIECE_KEYS[EMPTY][enPas];
}

inline int Board::getMaterial(const int side) const
{
    return material[side];
}

inline void Board::addMaterial(const int side, const int value)
{
    material[side] += value;
}

inline std::vector<Undo> Board::getHistory()
{
    return history;
}

inline void Board::addHistory(Undo &undo)
{
    history.push_back(undo);
}

inline Undo Board::popHistory()
{
    Undo undo = history.back();
    history.pop_back();
    return undo;
}

inline int Board::getPly() const
{
    return ply;
}

inline void Board::incrementPly()
{
    ply++;
}

inline void Board::decrementPly()
{
    ply--;
}

inline void Board::resetPly()
{
    ply = 0;
}

inline int Board::getSearchKiller(const int num) const
{
    return searchKillers[num][ply];
}

inline void Board::addSearchKiller(const int move)
{
    searchKillers[1][ply] = searchKillers[0][ply];
    searchKillers[0][ply] = move;
}

inline void Board::clearSearchKillers()
{
    for(int i = 0; i < 64; i++) {
        searchKillers[0][i] = 0;
        searchKillers[1][i] = 0;
    }
}

inline int Board::getSearchHistory(const int piece, const int square) const
{
    return searchHistory[piece][square];
}

inline void Board::incrementSearchHistory(const int to, const int from, const int depth)
{
    searchHistory[getSquare(from)][to] += depth;
}

inline void Board::clearSearchHistory()
{
    for(int i = 0; i < 13; i++) {
        for(int j = 0; j < 120; j++) {
            searchHistory[i][j] = 0;
        }
    }
}

#endif
