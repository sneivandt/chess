#pragma once

#include "board/undo.h"

#include <array>
#include <cstdint>
#include <iosfwd>
#include <string>
#include <type_traits>
#include <vector>

namespace board {

inline constexpr int BOARD_SQUARE_NUM = 120;
inline constexpr int BOARD64_SQUARE_NUM = 64;
inline constexpr int PIECE_TYPE_NUM = 13;
inline constexpr int COLOR_NUM = 3;
inline constexpr int SIDE_NUM = 2;
inline constexpr int FILE_NUM = 8;
inline constexpr int RANK_NUM = 8;
inline constexpr int MOVE_DIRECTION_NUM = 8;
inline constexpr int PIECE_LIST_CAPACITY = 10;
inline constexpr int CASTLE_PERM_NUM = 16;
inline constexpr int MAILBOX_FIRST_SQUARE = 21;
inline constexpr int MAILBOX_RANK_STRIDE = 10;
inline constexpr int MAX_FEN_HALFMOVE_CLOCK = 1000;
inline constexpr uint64_t ZOBRIST_SEED = 0xACEACEACEACEACEULL;

// Generic conversion to underlying integer type (equivalent to C++23 std::to_underlying)
template<typename E>
constexpr auto toInt(E e) noexcept -> std::underlying_type_t<E>
{
    return static_cast<std::underlying_type_t<E>>(e);
}

// Safe array index from int (suppresses sign-conversion warnings for non-negative indices)
constexpr std::size_t idx(int i) noexcept
{
    return static_cast<std::size_t>(i);
}

// Piece colors
enum class Color : int { WHITE = 0, BLACK = 1, BOTH = 2 };

// Pieces
enum class Piece : int { EMPTY = 0, WP, WN, WB, WR, WQ, WK, BP, BN, BB, BR, BQ, BK };

// Ranks
enum class Rank : int { RANK_1 = 0, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8, RANK_NONE };

// Files
enum class File : int { FILE_A = 0, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_NONE };

// Castling permissions (bitmask flags)
enum class CastlePerm : int { WKCA = 1, WQCA = 2, BKCA = 4, BQCA = 8 };

// CastlePerm bitwise operators
constexpr int operator|(CastlePerm a, CastlePerm b) noexcept
{
    return toInt(a) | toInt(b);
}
constexpr int operator|(int a, CastlePerm b) noexcept
{
    return a | toInt(b);
}
constexpr int operator&(int a, CastlePerm b) noexcept
{
    return a & toInt(b);
}
constexpr int& operator|=(int& a, CastlePerm b) noexcept
{
    a |= toInt(b);
    return a;
}

// Squares (120-square indexing)
enum class Square : int {
    A1 = 21,
    B1,
    C1,
    D1,
    E1,
    F1,
    G1,
    H1,
    A2 = 31,
    B2,
    C2,
    D2,
    E2,
    F2,
    G2,
    H2,
    A3 = 41,
    B3,
    C3,
    D3,
    E3,
    F3,
    G3,
    H3,
    A4 = 51,
    B4,
    C4,
    D4,
    E4,
    F4,
    G4,
    H4,
    A5 = 61,
    B5,
    C5,
    D5,
    E5,
    F5,
    G5,
    H5,
    A6 = 71,
    B6,
    C6,
    D6,
    E6,
    F6,
    G6,
    H6,
    A7 = 81,
    B7,
    C7,
    D7,
    E7,
    F7,
    G7,
    H7,
    A8 = 91,
    B8,
    C8,
    D8,
    E8,
    F8,
    G8,
    H8,
    NO_SQ = 99
};

// Off-board sentinel value for the 120-square board array
constexpr int OFFBOARD = toInt(Square::NO_SQ);

// Board representation
class Board
{
  private:
    std::array<int, BOARD_SQUARE_NUM> board;
    int side;
    int ply;
    int fiftyMove;
    int enPas;
    int castlePerm;
    uint64_t hashKey;
    std::array<uint64_t, COLOR_NUM> pawns;
    std::array<std::array<int, PIECE_LIST_CAPACITY>, PIECE_TYPE_NUM> pList;
    std::array<int, PIECE_TYPE_NUM> pNum;
    std::array<int, SIDE_NUM> material;
    std::vector<Undo> history;

  public:
    Board()
    {
        parseFen(DEFAULT_FEN);
    };
    Board(const std::string& fen)
    {
        parseFen(fen);
    };

    // Default position FEN
    static const char* DEFAULT_FEN;

    // Piece chars
    static const char* PIECE_CHARS;

    // Piece colors
    static const std::array<int, PIECE_TYPE_NUM> PIECE_COLOR;

    // Piece without team
    static const std::array<int, PIECE_TYPE_NUM> PIECE_NO_TEAM;

    // Piece values
    static const std::array<int, PIECE_TYPE_NUM> PIECE_VAL;

    // Ranks
    static const std::array<int, BOARD64_SQUARE_NUM> RANKS;

    // Files
    static const std::array<int, BOARD64_SQUARE_NUM> FILES;

    // Convert from a 120 to 64 board index
    static const std::array<int, BOARD_SQUARE_NUM> SQ64;

    // Convert from a 64 to 120 board index
    static const std::array<int, BOARD64_SQUARE_NUM> SQ120;

    // Castle perm mask
    static const std::array<int, BOARD_SQUARE_NUM> CASTLE_PERM_MASK;

    // Move directions
    static const std::array<std::array<int, MOVE_DIRECTION_NUM>, PIECE_TYPE_NUM> MOVE_DIR;

    // Zobrist hash keys
    static std::array<std::array<uint64_t, BOARD_SQUARE_NUM>, PIECE_TYPE_NUM> PIECE_KEYS;
    static std::array<uint64_t, CASTLE_PERM_NUM> CASTLE_KEYS;
    static uint64_t SIDE_KEY;

    // Bitmasks
    static std::array<uint64_t, RANK_NUM> RANK_MASK;
    static std::array<uint64_t, FILE_NUM> FILE_MASK;
    static std::array<std::array<uint64_t, BOARD64_SQUARE_NUM>, SIDE_NUM> PASSED_PAWN_MASK;
    static std::array<uint64_t, BOARD64_SQUARE_NUM> ISOLATED_PAWN_MASK;

    // Init
    static void INIT();

    // Get the square from a file and rank
    static int FR2SQ(const int, const int);

    // Reset the board
    void reset();

    // Parse a FEN string
    bool parseFen(const std::string&);

    // Print the board
    void print() const;
    void print(std::ostream&) const;

    // Check if a square is attacked
    bool sqAttacked(const int, const int) const;

    // Update lists and material
    bool updateListMaterial();

    // Board square
    int getSquare(const int) const;
    void setSquare(const int, const int);

    // Bitboards
    const std::array<uint64_t, COLOR_NUM>& getPawns() const;
    std::array<uint64_t, COLOR_NUM>& getPawns();

    // Piece lists
    void incrementPieceNum(const int);
    void decrementPieceNum(const int);
    int getPieceNum(const int) const;
    const std::array<int, PIECE_LIST_CAPACITY>& getPieceList(const int) const;
    std::array<int, PIECE_LIST_CAPACITY>& getPieceList(const int);

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
    const std::vector<Undo>& getHistory() const;
    void addHistory(const Undo&);
    Undo popHistory();

    // Search ply
    int getPly() const;
    void incrementPly();
    void decrementPly();
    void resetPly();
};

inline int Board::FR2SQ(const int file, const int rank)
{
    return (MAILBOX_FIRST_SQUARE + file) + (rank * MAILBOX_RANK_STRIDE);
}

inline int Board::getSquare(const int square) const
{
    return board[idx(square)];
}

inline void Board::setSquare(const int square, const int value)
{
    board[idx(square)] = value;
}

inline const std::array<uint64_t, COLOR_NUM>& Board::getPawns() const
{
    return pawns;
}

inline std::array<uint64_t, COLOR_NUM>& Board::getPawns()
{
    return pawns;
}

inline void Board::incrementPieceNum(const int piece)
{
    pNum[idx(piece)]++;
}

inline void Board::decrementPieceNum(const int piece)
{
    pNum[idx(piece)]--;
}

inline int Board::getPieceNum(const int piece) const
{
    return pNum[idx(piece)];
}

inline const std::array<int, PIECE_LIST_CAPACITY>& Board::getPieceList(const int piece) const
{
    return pList[idx(piece)];
}

inline std::array<int, PIECE_LIST_CAPACITY>& Board::getPieceList(const int piece)
{
    return pList[idx(piece)];
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
    enPas = toInt(Square::NO_SQ);
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
    castlePerm &= CASTLE_PERM_MASK[idx(to)] & CASTLE_PERM_MASK[idx(from)];
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
    hashKey ^= PIECE_KEYS[idx(piece)][idx(square)];
}

inline void Board::hashCastle()
{
    hashKey ^= CASTLE_KEYS[idx(castlePerm)];
}

inline void Board::hashEnPas()
{
    hashKey ^= PIECE_KEYS[idx(toInt(Piece::EMPTY))][idx(enPas)];
}

inline int Board::getMaterial(const int s) const
{
    return material[idx(s)];
}

inline void Board::addMaterial(const int s, const int value)
{
    material[idx(s)] += value;
}

inline const std::vector<Undo>& Board::getHistory() const
{
    return history;
}

inline void Board::addHistory(const Undo& undo)
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

} // namespace board
