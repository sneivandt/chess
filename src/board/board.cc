#include "board/board.h"

#include "bitboard.h"

#include <array>
#include <cstdint>
#include <iostream>
#include <limits>
#include <optional>
#include <random>
#include <sstream>
#include <string>
#include <string_view>

const char* board::Board::DEFAULT_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

const char* board::Board::PIECE_CHARS = ".PNBRQKpnbrqk";

const std::array<int, board::PIECE_TYPE_NUM> board::Board::PIECE_COLOR = {
    toInt(Color::BOTH),  toInt(Color::WHITE), toInt(Color::WHITE), toInt(Color::WHITE), toInt(Color::WHITE),
    toInt(Color::WHITE), toInt(Color::WHITE), toInt(Color::BLACK), toInt(Color::BLACK), toInt(Color::BLACK),
    toInt(Color::BLACK), toInt(Color::BLACK), toInt(Color::BLACK)};

const std::array<int, board::PIECE_TYPE_NUM> board::Board::PIECE_NO_TEAM = {
    toInt(Piece::EMPTY), toInt(Piece::WP), toInt(Piece::WN), toInt(Piece::WB), toInt(Piece::WR),
    toInt(Piece::WQ),    toInt(Piece::WK), toInt(Piece::WP), toInt(Piece::WN), toInt(Piece::WB),
    toInt(Piece::WR),    toInt(Piece::WQ), toInt(Piece::WK)};

const std::array<int, board::PIECE_TYPE_NUM> board::Board::PIECE_VAL = {0,   100, 300, 325, 500, 900, 0,
                                                                        100, 300, 325, 500, 900, 0};

const std::array<int, board::BOARD64_SQUARE_NUM> board::Board::RANKS = {
    toInt(Rank::RANK_1), toInt(Rank::RANK_1), toInt(Rank::RANK_1), toInt(Rank::RANK_1), toInt(Rank::RANK_1),
    toInt(Rank::RANK_1), toInt(Rank::RANK_1), toInt(Rank::RANK_1), toInt(Rank::RANK_2), toInt(Rank::RANK_2),
    toInt(Rank::RANK_2), toInt(Rank::RANK_2), toInt(Rank::RANK_2), toInt(Rank::RANK_2), toInt(Rank::RANK_2),
    toInt(Rank::RANK_2), toInt(Rank::RANK_3), toInt(Rank::RANK_3), toInt(Rank::RANK_3), toInt(Rank::RANK_3),
    toInt(Rank::RANK_3), toInt(Rank::RANK_3), toInt(Rank::RANK_3), toInt(Rank::RANK_3), toInt(Rank::RANK_4),
    toInt(Rank::RANK_4), toInt(Rank::RANK_4), toInt(Rank::RANK_4), toInt(Rank::RANK_4), toInt(Rank::RANK_4),
    toInt(Rank::RANK_4), toInt(Rank::RANK_4), toInt(Rank::RANK_5), toInt(Rank::RANK_5), toInt(Rank::RANK_5),
    toInt(Rank::RANK_5), toInt(Rank::RANK_5), toInt(Rank::RANK_5), toInt(Rank::RANK_5), toInt(Rank::RANK_5),
    toInt(Rank::RANK_6), toInt(Rank::RANK_6), toInt(Rank::RANK_6), toInt(Rank::RANK_6), toInt(Rank::RANK_6),
    toInt(Rank::RANK_6), toInt(Rank::RANK_6), toInt(Rank::RANK_6), toInt(Rank::RANK_7), toInt(Rank::RANK_7),
    toInt(Rank::RANK_7), toInt(Rank::RANK_7), toInt(Rank::RANK_7), toInt(Rank::RANK_7), toInt(Rank::RANK_7),
    toInt(Rank::RANK_7), toInt(Rank::RANK_8), toInt(Rank::RANK_8), toInt(Rank::RANK_8), toInt(Rank::RANK_8),
    toInt(Rank::RANK_8), toInt(Rank::RANK_8), toInt(Rank::RANK_8), toInt(Rank::RANK_8)};

const std::array<int, board::BOARD64_SQUARE_NUM> board::Board::FILES = {
    toInt(File::FILE_A), toInt(File::FILE_B), toInt(File::FILE_C), toInt(File::FILE_D), toInt(File::FILE_E),
    toInt(File::FILE_F), toInt(File::FILE_G), toInt(File::FILE_H), toInt(File::FILE_A), toInt(File::FILE_B),
    toInt(File::FILE_C), toInt(File::FILE_D), toInt(File::FILE_E), toInt(File::FILE_F), toInt(File::FILE_G),
    toInt(File::FILE_H), toInt(File::FILE_A), toInt(File::FILE_B), toInt(File::FILE_C), toInt(File::FILE_D),
    toInt(File::FILE_E), toInt(File::FILE_F), toInt(File::FILE_G), toInt(File::FILE_H), toInt(File::FILE_A),
    toInt(File::FILE_B), toInt(File::FILE_C), toInt(File::FILE_D), toInt(File::FILE_E), toInt(File::FILE_F),
    toInt(File::FILE_G), toInt(File::FILE_H), toInt(File::FILE_A), toInt(File::FILE_B), toInt(File::FILE_C),
    toInt(File::FILE_D), toInt(File::FILE_E), toInt(File::FILE_F), toInt(File::FILE_G), toInt(File::FILE_H),
    toInt(File::FILE_A), toInt(File::FILE_B), toInt(File::FILE_C), toInt(File::FILE_D), toInt(File::FILE_E),
    toInt(File::FILE_F), toInt(File::FILE_G), toInt(File::FILE_H), toInt(File::FILE_A), toInt(File::FILE_B),
    toInt(File::FILE_C), toInt(File::FILE_D), toInt(File::FILE_E), toInt(File::FILE_F), toInt(File::FILE_G),
    toInt(File::FILE_H), toInt(File::FILE_A), toInt(File::FILE_B), toInt(File::FILE_C), toInt(File::FILE_D),
    toInt(File::FILE_E), toInt(File::FILE_F), toInt(File::FILE_G), toInt(File::FILE_H)};

const std::array<int, board::BOARD_SQUARE_NUM> board::Board::SQ64 = {
    99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 0,  1,  2,
    3,  4,  5,  6,  7,  99, 99, 8,  9,  10, 11, 12, 13, 14, 15, 99, 99, 16, 17, 18, 19, 20, 21, 22,
    23, 99, 99, 24, 25, 26, 27, 28, 29, 30, 31, 99, 99, 32, 33, 34, 35, 36, 37, 38, 39, 99, 99, 40,
    41, 42, 43, 44, 45, 46, 47, 99, 99, 48, 49, 50, 51, 52, 53, 54, 55, 99, 99, 56, 57, 58, 59, 60,
    61, 62, 63, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99};

const std::array<int, board::BOARD64_SQUARE_NUM> board::Board::SQ120 = {
    21, 22, 23, 24, 25, 26, 27, 28, 31, 32, 33, 34, 35, 36, 37, 38, 41, 42, 43, 44, 45, 46,
    47, 48, 51, 52, 53, 54, 55, 56, 57, 58, 61, 62, 63, 64, 65, 66, 67, 68, 71, 72, 73, 74,
    75, 76, 77, 78, 81, 82, 83, 84, 85, 86, 87, 88, 91, 92, 93, 94, 95, 96, 97, 98};

const std::array<int, board::BOARD_SQUARE_NUM> board::Board::CASTLE_PERM_MASK = {
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 13, 15, 15,
    15, 12, 15, 15, 14, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 7,  15, 15, 15, 3,
    15, 15, 11, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15};

const std::array<std::array<int, board::MOVE_DIRECTION_NUM>, board::PIECE_TYPE_NUM> board::Board::MOVE_DIR = {
    {{0, 0, 0, 0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0, 0, 0, 0},
     {-8, -19, -21, -12, 8, 19, 21, 12},
     {-9, -11, 11, 9, 0, 0, 0, 0},
     {-1, -10, 1, 10, 0, 0, 0, 0},
     {-9, -11, 11, 9, -1, -10, 1, 10},
     {-9, -11, 11, 9, -1, -10, 1, 10},
     {0, 0, 0, 0, 0, 0, 0, 0},
     {-8, -19, -21, -12, 8, 19, 21, 12},
     {-9, -11, 11, 9, 0, 0, 0, 0},
     {-1, -10, 1, 10, 0, 0, 0, 0},
     {-9, -11, 11, 9, -1, -10, 1, 10},
     {-9, -11, 11, 9, -1, -10, 1, 10}}};

std::array<std::array<uint64_t, board::BOARD_SQUARE_NUM>, board::PIECE_TYPE_NUM> board::Board::PIECE_KEYS;

std::array<uint64_t, board::CASTLE_PERM_NUM> board::Board::CASTLE_KEYS;

uint64_t board::Board::SIDE_KEY;

std::array<uint64_t, board::FILE_NUM> board::Board::FILE_MASK;

std::array<uint64_t, board::RANK_NUM> board::Board::RANK_MASK;

std::array<std::array<uint64_t, board::BOARD64_SQUARE_NUM>, board::SIDE_NUM> board::Board::PASSED_PAWN_MASK;

std::array<uint64_t, board::BOARD64_SQUARE_NUM> board::Board::ISOLATED_PAWN_MASK;

namespace {

std::optional<int> pieceFromFen(const char c)
{
    switch (c) {
        case 'p':
            return board::toInt(board::Piece::BP);
        case 'r':
            return board::toInt(board::Piece::BR);
        case 'n':
            return board::toInt(board::Piece::BN);
        case 'b':
            return board::toInt(board::Piece::BB);
        case 'q':
            return board::toInt(board::Piece::BQ);
        case 'k':
            return board::toInt(board::Piece::BK);
        case 'P':
            return board::toInt(board::Piece::WP);
        case 'R':
            return board::toInt(board::Piece::WR);
        case 'N':
            return board::toInt(board::Piece::WN);
        case 'B':
            return board::toInt(board::Piece::WB);
        case 'Q':
            return board::toInt(board::Piece::WQ);
        case 'K':
            return board::toInt(board::Piece::WK);
        default:
            return std::nullopt;
    }
}

bool parseNonNegativeInt(const std::string_view token, int& value, const std::optional<int> cap = std::nullopt)
{
    if (token.empty()) {
        return false;
    }

    int parsed = 0;
    bool saturated = false;
    for (const char c : token) {
        if (c < '0' || c > '9') {
            return false;
        }

        if (saturated) {
            continue;
        }

        const int digit = c - '0';
        const int maxValue = cap.value_or(std::numeric_limits<int>::max());
        if (parsed > (maxValue - digit) / 10) {
            if (!cap) {
                return false;
            }
            parsed = maxValue;
            saturated = true;
            continue;
        }
        parsed = (parsed * 10) + digit;
    }

    if (cap && parsed > *cap) {
        parsed = *cap;
    }
    value = parsed;
    return true;
}

bool parsePlacement(const std::string_view placement, board::Board& pos)
{
    int rank = board::toInt(board::Rank::RANK_8);
    int file = board::toInt(board::File::FILE_A);

    for (const char c : placement) {
        if (c == '/') {
            if (file != board::FILE_NUM || rank == board::toInt(board::Rank::RANK_1)) {
                return false;
            }
            --rank;
            file = board::toInt(board::File::FILE_A);
            continue;
        }

        if (c >= '1' && c <= '8') {
            file += c - '0';
            if (file > board::FILE_NUM) {
                return false;
            }
            continue;
        }

        const auto piece = pieceFromFen(c);
        if (!piece || file >= board::FILE_NUM) {
            return false;
        }

        pos.setSquare(board::Board::FR2SQ(file, rank), *piece);
        ++file;
    }

    return rank == board::toInt(board::Rank::RANK_1) && file == board::FILE_NUM;
}

std::optional<int> parseSide(const std::string_view token)
{
    if (token == "w") {
        return board::toInt(board::Color::WHITE);
    }
    if (token == "b") {
        return board::toInt(board::Color::BLACK);
    }
    return std::nullopt;
}

std::optional<int> parseCastling(const std::string_view token)
{
    if (token == "-") {
        return 0;
    }
    if (token.empty() || token.size() > 4) {
        return std::nullopt;
    }

    int castlePerm = 0;
    for (const char c : token) {
        int flag = 0;
        switch (c) {
            case 'K':
                flag = board::toInt(board::CastlePerm::WKCA);
                break;
            case 'Q':
                flag = board::toInt(board::CastlePerm::WQCA);
                break;
            case 'k':
                flag = board::toInt(board::CastlePerm::BKCA);
                break;
            case 'q':
                flag = board::toInt(board::CastlePerm::BQCA);
                break;
            default:
                return std::nullopt;
        }
        if ((castlePerm & flag) != 0) {
            return std::nullopt;
        }
        castlePerm |= flag;
    }
    return castlePerm;
}

std::optional<int> parseEnPassant(const std::string_view token)
{
    if (token == "-") {
        return board::toInt(board::Square::NO_SQ);
    }
    if (token.size() != 2) {
        return std::nullopt;
    }

    const int file = token[0] - 'a';
    const int rank = token[1] - '1';
    if (file < board::toInt(board::File::FILE_A) || file > board::toInt(board::File::FILE_H) ||
        rank < board::toInt(board::Rank::RANK_1) || rank > board::toInt(board::Rank::RANK_8)) {
        return std::nullopt;
    }
    if (rank != board::toInt(board::Rank::RANK_3) && rank != board::toInt(board::Rank::RANK_6)) {
        return std::nullopt;
    }

    return board::Board::FR2SQ(file, rank);
}

} // namespace

void board::Board::INIT()
{
    // Zobrist hash keys
    std::mt19937_64 e2(ZOBRIST_SEED);
    std::uniform_int_distribution<uint64_t> dist;
    for (auto& i : PIECE_KEYS) {
        for (auto& j : i) {
            j = dist(e2);
        }
    }
    for (auto& key : CASTLE_KEYS) {
        key = dist(e2);
    }
    SIDE_KEY = dist(e2);
    // Bitmasks
    int square;
    int tSquare;
    for (int i = 0; i < FILE_NUM; i++) {
        FILE_MASK[idx(i)] = 0ULL;
        RANK_MASK[idx(i)] = 0ULL;
    }
    for (int i = 0; i < BOARD64_SQUARE_NUM; i++) {
        PASSED_PAWN_MASK[0][idx(i)] = 0ULL;
        PASSED_PAWN_MASK[1][idx(i)] = 0ULL;
        ISOLATED_PAWN_MASK[idx(i)] = 0ULL;
    }
    for (int rank = toInt(Rank::RANK_1); rank <= toInt(Rank::RANK_8); rank++) {
        for (int file = toInt(File::FILE_A); file <= toInt(File::FILE_H); file++) {
            square = rank * FILE_NUM + file;
            FILE_MASK[idx(file)] |= (1ULL << square);
            RANK_MASK[idx(rank)] |= (1ULL << square);
        }
    }
    for (square = 0; square < BOARD64_SQUARE_NUM; square++) {
        tSquare = square + FILE_NUM;
        while (tSquare < BOARD64_SQUARE_NUM) {
            PASSED_PAWN_MASK[idx(toInt(Color::WHITE))][idx(square)] |= (1ULL << tSquare);
            tSquare += FILE_NUM;
        }
        tSquare = square - FILE_NUM;
        while (tSquare >= 0) {
            PASSED_PAWN_MASK[idx(toInt(Color::BLACK))][idx(square)] |= (1ULL << tSquare);
            tSquare -= FILE_NUM;
        }
        if (Board::FILES[idx(square)] > toInt(File::FILE_A)) {
            ISOLATED_PAWN_MASK[idx(square)] |= FILE_MASK[idx(Board::FILES[idx(square)] - 1)];
            tSquare = square + FILE_NUM - 1;
            while (tSquare < BOARD64_SQUARE_NUM) {
                PASSED_PAWN_MASK[idx(toInt(Color::WHITE))][idx(square)] |= (1ULL << tSquare);
                tSquare += FILE_NUM;
            }
            tSquare = square - FILE_NUM - 1;
            while (tSquare >= 0) {
                PASSED_PAWN_MASK[idx(toInt(Color::BLACK))][idx(square)] |= (1ULL << tSquare);
                tSquare -= FILE_NUM;
            }
        }
        if (Board::FILES[idx(square)] < toInt(File::FILE_H)) {
            ISOLATED_PAWN_MASK[idx(square)] |= FILE_MASK[idx(Board::FILES[idx(square)] + 1)];
            tSquare = square + FILE_NUM + 1;
            while (tSquare < BOARD64_SQUARE_NUM) {
                PASSED_PAWN_MASK[idx(toInt(Color::WHITE))][idx(square)] |= (1ULL << tSquare);
                tSquare += FILE_NUM;
            }
            tSquare = square - FILE_NUM + 1;
            while (tSquare >= 0) {
                PASSED_PAWN_MASK[idx(toInt(Color::BLACK))][idx(square)] |= (1ULL << tSquare);
                tSquare -= FILE_NUM;
            }
        }
    }
}

void board::Board::reset()
{
    board.fill(OFFBOARD);
    for (int i : SQ120) {
        board[idx(i)] = toInt(Piece::EMPTY);
    }
    pawns.fill(0ULL);
    for (int i = 0; i < PIECE_TYPE_NUM; i++) {
        pNum[idx(i)] = 0;
        pList[idx(i)].fill(0);
    }
    material[toInt(Color::WHITE)] = 0;
    material[toInt(Color::BLACK)] = 0;
    side = toInt(Color::BOTH);
    ply = 0;
    fiftyMove = 0;
    enPas = toInt(Square::NO_SQ);
    castlePerm = 0;
    hashKey = 0ULL;
    history.clear();
}

bool board::Board::parseFen(const std::string& fen)
{
    reset();
    std::array<std::string, 6> fields;
    std::istringstream stream(fen);
    for (auto& field : fields) {
        if (!(stream >> field)) {
            return false;
        }
    }
    std::string extra;
    if (stream >> extra) {
        return false;
    }

    if (!parsePlacement(fields[0], *this)) {
        return false;
    }

    const auto parsedSide = parseSide(fields[1]);
    if (!parsedSide) {
        return false;
    }
    side = *parsedSide;

    const auto parsedCastlePerm = parseCastling(fields[2]);
    if (!parsedCastlePerm) {
        return false;
    }
    castlePerm = *parsedCastlePerm;

    const auto parsedEnPas = parseEnPassant(fields[3]);
    if (!parsedEnPas) {
        return false;
    }
    enPas = *parsedEnPas;

    if (!parseNonNegativeInt(fields[4], fiftyMove, MAX_FEN_HALFMOVE_CLOCK)) {
        return false;
    }

    int fullMove = 0;
    if (!parseNonNegativeInt(fields[5], fullMove) || fullMove == 0) {
        return false;
    }

    if (!updateListMaterial()) {
        return false; // Invalid position: too many pieces of one type
    }
    generateHash();
    return true;
}

void board::Board::print() const
{
    print(std::cout);
}

void board::Board::print(std::ostream& output) const
{
    int square;
    int piece;
    if (side == toInt(Color::WHITE)) {
        output << "     white" << std::endl;
    }
    else {
        output << "     black" << std::endl;
    }
    for (int rank = toInt(Rank::RANK_8); rank >= toInt(Rank::RANK_1); rank--) {
        for (int file = toInt(File::FILE_A); file <= toInt(File::FILE_H); file++) {
            square = FR2SQ(file, rank);
            piece = board[idx(square)];
            output << PIECE_CHARS[idx(piece)] << " ";
        }
        output << std::endl;
    }
}

bool board::Board::sqAttacked(const int square, const int side_in) const
{
    // Guard against off-board squares (e.g. when no king exists)
    if (square < 21 || square > 98 || board[idx(square)] == OFFBOARD) {
        return false;
    }

    int targetSquare;
    int piece;

    // Pawn attacks
    if (side_in == toInt(Color::WHITE)) {
        if (board[idx(square - 11)] == toInt(Piece::WP) || board[idx(square - 9)] == toInt(Piece::WP))
            return true;
    }
    else {
        if (board[idx(square + 11)] == toInt(Piece::BP) || board[idx(square + 9)] == toInt(Piece::BP))
            return true;
    }

    // Knight attacks
    for (int i = 0; i < MOVE_DIRECTION_NUM; i++) {
        piece = board[idx(square + MOVE_DIR[idx(toInt(Piece::WN))][idx(i)])];
        if (side_in == toInt(Color::WHITE) && piece == toInt(Piece::WN)) {
            return true;
        }
        if (side_in == toInt(Color::BLACK) && piece == toInt(Piece::BN)) {
            return true;
        }
    }

    // Rook / Queen attacks
    for (int i = 0; i < 4; i++) {
        targetSquare = square + MOVE_DIR[idx(toInt(Piece::WR))][idx(i)];
        piece = board[idx(targetSquare)];
        while (piece != OFFBOARD) {
            if (piece != toInt(Piece::EMPTY)) {
                if (side_in == toInt(Color::WHITE) && (piece == toInt(Piece::WR) || piece == toInt(Piece::WQ))) {
                    return true;
                }
                if (side_in == toInt(Color::BLACK) && (piece == toInt(Piece::BR) || piece == toInt(Piece::BQ))) {
                    return true;
                }
                break;
            }
            targetSquare += MOVE_DIR[idx(toInt(Piece::WR))][idx(i)];
            piece = board[idx(targetSquare)];
        }
    }

    // Bishop / Queen attacks
    for (int i = 0; i < 4; i++) {
        targetSquare = square + MOVE_DIR[idx(toInt(Piece::WB))][idx(i)];
        piece = board[idx(targetSquare)];
        while (piece != OFFBOARD) {
            if (piece != toInt(Piece::EMPTY)) {
                if (side_in == toInt(Color::WHITE) && (piece == toInt(Piece::WB) || piece == toInt(Piece::WQ))) {
                    return true;
                }
                if (side_in == toInt(Color::BLACK) && (piece == toInt(Piece::BB) || piece == toInt(Piece::BQ))) {
                    return true;
                }
                break;
            }
            targetSquare += MOVE_DIR[idx(toInt(Piece::WB))][idx(i)];
            piece = board[idx(targetSquare)];
        }
    }

    // King attacks
    for (int i = 0; i < MOVE_DIRECTION_NUM; i++) {
        piece = board[idx(square + MOVE_DIR[idx(toInt(Piece::WK))][idx(i)])];
        if ((side_in == toInt(Color::WHITE) && piece == toInt(Piece::WK)) ||
            (side_in == toInt(Color::BLACK) && piece == toInt(Piece::BK))) {
            return true;
        }
    }
    return false;
}

bool board::Board::updateListMaterial()
{
    int piece;
    for (int i = 0; i < BOARD_SQUARE_NUM; i++) {
        piece = board[idx(i)];
        if (piece != OFFBOARD && piece != toInt(Piece::EMPTY)) {
            // Check bounds before adding to piece list
            if (pNum[idx(piece)] >= PIECE_LIST_CAPACITY) {
                return false; // Return false to indicate overflow
            }
            pList[idx(piece)][idx(pNum[idx(piece)])] = i;
            pNum[idx(piece)]++;
            material[idx(PIECE_COLOR[idx(piece)])] += PIECE_VAL[idx(piece)];
            if (piece == toInt(Piece::WP)) {
                board::bitboard::setBit(pawns[idx(toInt(Color::WHITE))], SQ64[idx(i)]);
                board::bitboard::setBit(pawns[idx(toInt(Color::BOTH))], SQ64[idx(i)]);
            }
            else if (piece == toInt(Piece::BP)) {
                board::bitboard::setBit(pawns[idx(toInt(Color::BLACK))], SQ64[idx(i)]);
                board::bitboard::setBit(pawns[idx(toInt(Color::BOTH))], SQ64[idx(i)]);
            }
        }
    }
    return true; // Success
}

void board::Board::generateHash()
{
    uint64_t key = 0;
    int piece;
    for (int square = 0; square < BOARD_SQUARE_NUM; square++) {
        piece = board[idx(square)];
        if (piece != OFFBOARD && piece != toInt(Piece::EMPTY)) {
            key ^= PIECE_KEYS[idx(piece)][idx(square)];
        }
    }
    if (enPas != toInt(Square::NO_SQ)) {
        key ^= PIECE_KEYS[idx(toInt(Piece::EMPTY))][idx(enPas)];
    }
    if (side == toInt(Color::WHITE)) {
        key ^= SIDE_KEY;
    }
    key ^= CASTLE_KEYS[idx(castlePerm)];
    hashKey = key;
}
