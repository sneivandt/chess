#include "board/board.h"

#include "bitboard.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <random>

const char* board::Board::DEFAULT_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

const char* board::Board::PIECE_CHARS = ".PNBRQKpnbrqk";

const std::array<int, 13> board::Board::PIECE_COLOR = {
    toInt(Color::BOTH),  toInt(Color::WHITE), toInt(Color::WHITE), toInt(Color::WHITE), toInt(Color::WHITE),
    toInt(Color::WHITE), toInt(Color::WHITE), toInt(Color::BLACK), toInt(Color::BLACK), toInt(Color::BLACK),
    toInt(Color::BLACK), toInt(Color::BLACK), toInt(Color::BLACK)};

const std::array<int, 13> board::Board::PIECE_NO_TEAM = {
    toInt(Piece::EMPTY), toInt(Piece::WP), toInt(Piece::WN), toInt(Piece::WB), toInt(Piece::WR),
    toInt(Piece::WQ),    toInt(Piece::WK), toInt(Piece::WP), toInt(Piece::WN), toInt(Piece::WB),
    toInt(Piece::WR),    toInt(Piece::WQ), toInt(Piece::WK)};

const std::array<int, 13> board::Board::PIECE_VAL = {0, 100, 300, 325, 500, 900, 0, 100, 300, 325, 500, 900, 0};

const std::array<int, 64> board::Board::RANKS = {
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

const std::array<int, 64> board::Board::FILES = {
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

const std::array<int, 120> board::Board::SQ64 = {
    99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 0,  1,  2,
    3,  4,  5,  6,  7,  99, 99, 8,  9,  10, 11, 12, 13, 14, 15, 99, 99, 16, 17, 18, 19, 20, 21, 22,
    23, 99, 99, 24, 25, 26, 27, 28, 29, 30, 31, 99, 99, 32, 33, 34, 35, 36, 37, 38, 39, 99, 99, 40,
    41, 42, 43, 44, 45, 46, 47, 99, 99, 48, 49, 50, 51, 52, 53, 54, 55, 99, 99, 56, 57, 58, 59, 60,
    61, 62, 63, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99};

const std::array<int, 64> board::Board::SQ120 = {21, 22, 23, 24, 25, 26, 27, 28, 31, 32, 33, 34, 35, 36, 37, 38,
                                                 41, 42, 43, 44, 45, 46, 47, 48, 51, 52, 53, 54, 55, 56, 57, 58,
                                                 61, 62, 63, 64, 65, 66, 67, 68, 71, 72, 73, 74, 75, 76, 77, 78,
                                                 81, 82, 83, 84, 85, 86, 87, 88, 91, 92, 93, 94, 95, 96, 97, 98};

const std::array<int, 120> board::Board::CASTLE_PERM_MASK = {
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 13, 15, 15,
    15, 12, 15, 15, 14, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 7,  15, 15, 15, 3,
    15, 15, 11, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15};

const std::array<std::array<int, 8>, 13> board::Board::MOVE_DIR = {{{0, 0, 0, 0, 0, 0, 0, 0},
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

std::array<std::array<uint64_t, 120>, 13> board::Board::PIECE_KEYS;

std::array<uint64_t, 16> board::Board::CASTLE_KEYS;

uint64_t board::Board::SIDE_KEY;

std::array<uint64_t, 8> board::Board::FILE_MASK;

std::array<uint64_t, 8> board::Board::RANK_MASK;

std::array<std::array<uint64_t, 64>, 2> board::Board::PASSED_PAWN_MASK;

std::array<uint64_t, 64> board::Board::ISOLATED_PAWN_MASK;

void board::Board::INIT()
{
    // Zobrist hash keys
    std::random_device rd;
    std::mt19937_64 e2(rd());
    std::uniform_int_distribution<uint64_t> dist(std::llround(std::pow(2, 61)), std::llround(std::pow(2, 62)));
    for (auto& i : PIECE_KEYS) {
        for (auto& j : i) {
            j = dist(e2);
        }
    }
    std::fill(std::begin(CASTLE_KEYS), std::end(CASTLE_KEYS), dist(e2));
    SIDE_KEY = dist(e2);
    // Bitmasks
    int square;
    int tSquare;
    for (int i = 0; i < 8; i++) {
        FILE_MASK[idx(i)] = 0ULL;
        RANK_MASK[idx(i)] = 0ULL;
    }
    for (int i = 0; i < 64; i++) {
        PASSED_PAWN_MASK[0][idx(i)] = 0ULL;
        PASSED_PAWN_MASK[1][idx(i)] = 0ULL;
        ISOLATED_PAWN_MASK[idx(i)] = 0ULL;
    }
    for (int rank = toInt(Rank::RANK_1); rank <= toInt(Rank::RANK_8); rank++) {
        for (int file = toInt(File::FILE_A); file <= toInt(File::FILE_H); file++) {
            square = rank * 8 + file;
            FILE_MASK[idx(file)] |= (1ULL << square);
            RANK_MASK[idx(rank)] |= (1ULL << square);
        }
    }
    for (square = 0; square < 64; square++) {
        tSquare = square + 8;
        while (tSquare < 64) {
            PASSED_PAWN_MASK[idx(toInt(Color::WHITE))][idx(square)] |= (1ULL << tSquare);
            tSquare += 8;
        }
        tSquare = square - 8;
        while (tSquare >= 0) {
            PASSED_PAWN_MASK[idx(toInt(Color::BLACK))][idx(square)] |= (1ULL << tSquare);
            tSquare -= 8;
        }
        if (Board::FILES[idx(square)] > toInt(File::FILE_A)) {
            ISOLATED_PAWN_MASK[idx(square)] |= FILE_MASK[idx(Board::FILES[idx(square)] - 1)];
            tSquare = square + 7;
            while (tSquare < 64) {
                PASSED_PAWN_MASK[idx(toInt(Color::WHITE))][idx(square)] |= (1ULL << tSquare);
                tSquare += 8;
            }
            tSquare = square - 9;
            while (tSquare >= 0) {
                PASSED_PAWN_MASK[idx(toInt(Color::BLACK))][idx(square)] |= (1ULL << tSquare);
                tSquare -= 8;
            }
        }
        if (Board::FILES[idx(square)] < toInt(File::FILE_H)) {
            ISOLATED_PAWN_MASK[idx(square)] |= FILE_MASK[idx(Board::FILES[idx(square)] + 1)];
            tSquare = square + 9;
            while (tSquare < 64) {
                PASSED_PAWN_MASK[idx(toInt(Color::WHITE))][idx(square)] |= (1ULL << tSquare);
                tSquare += 8;
            }
            tSquare = square - 7;
            while (tSquare >= 0) {
                PASSED_PAWN_MASK[idx(toInt(Color::BLACK))][idx(square)] |= (1ULL << tSquare);
                tSquare -= 8;
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
    for (int i = 0; i < 13; i++) {
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
    int piece = toInt(Piece::EMPTY);
    int sq64 = 0;
    int sq120 = 0;
    int rank = toInt(Rank::RANK_8);
    int file = toInt(File::FILE_A);
    size_t index = 0;

    // Parse piece placement
    while (rank >= toInt(Rank::RANK_1) && index < fen.length()) {
        int count = 1;
        char c = fen[index];
        piece = toInt(Piece::EMPTY);

        switch (c) {
            case 'p':
                piece = toInt(Piece::BP);
                break;
            case 'r':
                piece = toInt(Piece::BR);
                break;
            case 'n':
                piece = toInt(Piece::BN);
                break;
            case 'b':
                piece = toInt(Piece::BB);
                break;
            case 'q':
                piece = toInt(Piece::BQ);
                break;
            case 'k':
                piece = toInt(Piece::BK);
                break;
            case 'P':
                piece = toInt(Piece::WP);
                break;
            case 'R':
                piece = toInt(Piece::WR);
                break;
            case 'N':
                piece = toInt(Piece::WN);
                break;
            case 'B':
                piece = toInt(Piece::WB);
                break;
            case 'Q':
                piece = toInt(Piece::WQ);
                break;
            case 'K':
                piece = toInt(Piece::WK);
                break;
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
                piece = toInt(Piece::EMPTY);
                count = c - '0';
                break;
            case '/':
            case ' ':
                rank--;
                file = toInt(File::FILE_A);
                index++;
                continue;
            default:
                return false;
        }

        for (int i = 0; i < count; i++) {
            if (file > toInt(File::FILE_H)) {
                return false; // Too many files in this rank
            }
            sq64 = rank * 8 + file;
            if (sq64 >= 0 && sq64 < 64) {
                sq120 = SQ120[idx(sq64)];
                if (piece != toInt(Piece::EMPTY)) {
                    board[idx(sq120)] = piece;
                }
            }
            file++;
        }
        index++;
    }

    // Check if we finished the board part
    // The previous loop decrements rank when it hits ' ' or '/', so we need to find the space separator
    // actually, my loop logic for ' ' decrements rank effectively exiting the loop.
    // We should find the next part of the FEN string.

    // Recover index position if we overshot or loop terminated
    // We need to find the space that separates board from side
    size_t spacePos = fen.find(' ', 0);
    if (spacePos == std::string::npos)
        return false;

    index = spacePos + 1;
    if (index >= fen.length())
        return false;

    side = fen[index] == 'w' ? toInt(Color::WHITE) : toInt(Color::BLACK);
    index += 2;

    for (int i = 0; i < 4; i++) {
        if (index >= fen.length() || fen[index] == ' ') {
            break;
        }
        switch (fen[index]) {
            case 'K':
                castlePerm |= CastlePerm::WKCA;
                break;
            case 'Q':
                castlePerm |= CastlePerm::WQCA;
                break;
            case 'k':
                castlePerm |= CastlePerm::BKCA;
                break;
            case 'q':
                castlePerm |= CastlePerm::BQCA;
                break;
        }
        index++;
    }
    index++;

    if (index < fen.length() && fen[index] != '-') {
        if (index + 1 < fen.length()) { // Check bounds for valid EnPas parsing
            file = fen[index] - 'a';
            rank = fen[index + 1] - '1';
            // Validate en passant square is within valid range
            if (file < toInt(File::FILE_A) || file > toInt(File::FILE_H) || rank < toInt(Rank::RANK_1) ||
                rank > toInt(Rank::RANK_8)) {
                return false; // Invalid en passant square
            }
            enPas = FR2SQ(file, rank);
            index += 2; // Skip both characters of en passant square
        }
    }
    else {
        index++; // Skip the '-' character
    }
    index++; // Skip space

    while (index < fen.length() && fen[index] != ' ') {
        int digit = fen[index] - '0';
        // Validate digit range
        if (digit < 0 || digit > 9) {
            break;
        }
        // Check if the next multiplication and addition would exceed 1000
        if (fiftyMove * 10 + digit > 1000) {
            fiftyMove = 1000; // Cap at reasonable maximum
            break;
        }
        fiftyMove *= 10;
        fiftyMove += digit;
        index++;
    }

    // ply parsing would follow but seems commented out or missing in original logic

    if (!updateListMaterial()) {
        return false; // Invalid position: too many pieces of one type
    }
    generateHash();
    return true;
}

void board::Board::print() const
{
    int square;
    int piece;
    if (side == toInt(Color::WHITE)) {
        std::cout << "     white" << std::endl;
    }
    else {
        std::cout << "     black" << std::endl;
    }
    for (int rank = toInt(Rank::RANK_8); rank >= toInt(Rank::RANK_1); rank--) {
        for (int file = toInt(File::FILE_A); file <= toInt(File::FILE_H); file++) {
            square = FR2SQ(file, rank);
            piece = board[idx(square)];
            std::cout << PIECE_CHARS[idx(piece)] << " ";
        }
        std::cout << std::endl;
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
    for (int i = 0; i < 8; i++) {
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
    for (int i = 0; i < 8; i++) {
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
    for (int i = 0; i < 120; i++) {
        piece = board[idx(i)];
        if (piece != OFFBOARD && piece != toInt(Piece::EMPTY)) {
            // Check bounds before adding to piece list
            if (pNum[idx(piece)] >= 10) {
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
    for (int square = 0; square < 120; square++) {
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
    hashKey = key;
}
