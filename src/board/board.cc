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

const int board::Board::PIECE_COLOR[13] = {BOTH,  WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
                                           BLACK, BLACK, BLACK, BLACK, BLACK, BLACK};

const int board::Board::PIECE_NO_TEAM[13] = {EMPTY, WP, WN, WB, WR, WQ, WK, WP, WN, WB, WR, WQ, WK};

const int board::Board::PIECE_VAL[13] = {0, 100, 300, 325, 500, 900, 0, 100, 300, 325, 500, 900, 0};

const int board::Board::RANKS[64] = {
    RANK_1, RANK_1, RANK_1, RANK_1, RANK_1, RANK_1, RANK_1, RANK_1, RANK_2, RANK_2, RANK_2, RANK_2, RANK_2,
    RANK_2, RANK_2, RANK_2, RANK_3, RANK_3, RANK_3, RANK_3, RANK_3, RANK_3, RANK_3, RANK_3, RANK_4, RANK_4,
    RANK_4, RANK_4, RANK_4, RANK_4, RANK_4, RANK_4, RANK_5, RANK_5, RANK_5, RANK_5, RANK_5, RANK_5, RANK_5,
    RANK_5, RANK_6, RANK_6, RANK_6, RANK_6, RANK_6, RANK_6, RANK_6, RANK_6, RANK_7, RANK_7, RANK_7, RANK_7,
    RANK_7, RANK_7, RANK_7, RANK_7, RANK_8, RANK_8, RANK_8, RANK_8, RANK_8, RANK_8, RANK_8, RANK_8};

const int board::Board::FILES[64] = {
    FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_A, FILE_B, FILE_C, FILE_D, FILE_E,
    FILE_F, FILE_G, FILE_H, FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_A, FILE_B,
    FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G,
    FILE_H, FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_A, FILE_B, FILE_C, FILE_D,
    FILE_E, FILE_F, FILE_G, FILE_H, FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H};

const int board::Board::SQ64[120] = {99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
                                     99, 0,  1,  2,  3,  4,  5,  6,  7,  99, 99, 8,  9,  10, 11, 12, 13, 14, 15, 99,
                                     99, 16, 17, 18, 19, 20, 21, 22, 23, 99, 99, 24, 25, 26, 27, 28, 29, 30, 31, 99,
                                     99, 32, 33, 34, 35, 36, 37, 38, 39, 99, 99, 40, 41, 42, 43, 44, 45, 46, 47, 99,
                                     99, 48, 49, 50, 51, 52, 53, 54, 55, 99, 99, 56, 57, 58, 59, 60, 61, 62, 63, 99,
                                     99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99};

const int board::Board::SQ120[64] = {21, 22, 23, 24, 25, 26, 27, 28, 31, 32, 33, 34, 35, 36, 37, 38,
                                     41, 42, 43, 44, 45, 46, 47, 48, 51, 52, 53, 54, 55, 56, 57, 58,
                                     61, 62, 63, 64, 65, 66, 67, 68, 71, 72, 73, 74, 75, 76, 77, 78,
                                     81, 82, 83, 84, 85, 86, 87, 88, 91, 92, 93, 94, 95, 96, 97, 98};

const int board::Board::CASTLE_PERM_MASK[120] = {
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 13, 15, 15,
    15, 12, 15, 15, 14, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 7,  15, 15, 15, 3,
    15, 15, 11, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15};

const int board::Board::MOVE_DIR[13][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},         {0, 0, 0, 0, 0, 0, 0, 0},     {-8, -19, -21, -12, 8, 19, 21, 12},
    {-9, -11, 11, 9, 0, 0, 0, 0},     {-1, -10, 1, 10, 0, 0, 0, 0}, {-9, -11, 11, 9, -1, -10, 1, 10},
    {-9, -11, 11, 9, -1, -10, 1, 10}, {0, 0, 0, 0, 0, 0, 0, 0},     {-8, -19, -21, -12, 8, 19, 21, 12},
    {-9, -11, 11, 9, 0, 0, 0, 0},     {-1, -10, 1, 10, 0, 0, 0, 0}, {-9, -11, 11, 9, -1, -10, 1, 10},
    {-9, -11, 11, 9, -1, -10, 1, 10}};

uint64_t board::Board::PIECE_KEYS[13][120];

uint64_t board::Board::CASTLE_KEYS[16];

uint64_t board::Board::SIDE_KEY;

uint64_t board::Board::FILE_MASK[8];

uint64_t board::Board::RANK_MASK[8];

uint64_t board::Board::PASSED_PAWN_MASK[2][64];

uint64_t board::Board::ISOLATED_PAWN_MASK[64];

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
        FILE_MASK[i] = 0ULL;
        RANK_MASK[i] = 0ULL;
    }
    for (int i = 0; i < 64; i++) {
        PASSED_PAWN_MASK[0][i] = 0ULL;
        PASSED_PAWN_MASK[1][i] = 0ULL;
        ISOLATED_PAWN_MASK[i] = 0ULL;
    }
    for (int rank = RANK_1; rank <= RANK_8; rank++) {
        for (int file = FILE_A; file <= FILE_H; file++) {
            square = rank * 8 + file;
            FILE_MASK[file] |= (1ULL << square);
            RANK_MASK[rank] |= (1ULL << square);
        }
    }
    for (square = 0; square < 64; square++) {
        tSquare = square + 8;
        while (tSquare < 64) {
            PASSED_PAWN_MASK[WHITE][square] |= (1ULL << tSquare);
            tSquare += 8;
        }
        tSquare = square - 8;
        while (tSquare >= 0) {
            PASSED_PAWN_MASK[BLACK][square] |= (1ULL << tSquare);
            tSquare -= 8;
        }
        if (Board::FILES[square] > FILE_A) {
            ISOLATED_PAWN_MASK[square] |= FILE_MASK[Board::FILES[square] - 1];
            tSquare = square + 7;
            while (tSquare < 64) {
                PASSED_PAWN_MASK[WHITE][square] |= (1ULL << tSquare);
                tSquare += 8;
            }
            tSquare = square - 9;
            while (tSquare >= 0) {
                PASSED_PAWN_MASK[BLACK][square] |= (1ULL << tSquare);
                tSquare -= 8;
            }
        }
        if (Board::FILES[square] < FILE_H) {
            ISOLATED_PAWN_MASK[square] |= FILE_MASK[Board::FILES[square] + 1];
            tSquare = square + 9;
            while (tSquare < 64) {
                PASSED_PAWN_MASK[WHITE][square] |= (1ULL << tSquare);
                tSquare += 8;
            }
            tSquare = square - 7;
            while (tSquare >= 0) {
                PASSED_PAWN_MASK[BLACK][square] |= (1ULL << tSquare);
                tSquare -= 8;
            }
        }
    }
}

void board::Board::reset()
{
    std::fill(std::begin(board), std::end(board), NO_SQ);
    for (int i : SQ120) {
        board[i] = EMPTY;
    }
    std::fill(std::begin(pawns), std::end(pawns), 0ULL);
    for (int i = 0; i < 13; i++) {
        pNum[i] = 0;
        for (int j = 0; j < 10; j++) {
            pList[i][j] = 0;
        }
    }
    material[WHITE] = 0;
    material[BLACK] = 0;
    side = BOTH;
    ply = 0;
    fiftyMove = 0;
    enPas = NO_SQ;
    castlePerm = 0;
    hashKey = 0ULL;
    history.clear();
    clearSearchKillers();
    clearSearchHistory();
}

bool board::Board::parseFen(const std::string& fen)
{
    reset();
    int piece = EMPTY;
    int sq64 = 0;
    int sq120 = 0;
    int rank = RANK_8;
    int file = FILE_A;
    size_t index = 0;

    // Parse piece placement
    while (rank >= RANK_1 && index < fen.length()) {
        int count = 1;
        char c = fen[index];
        piece = EMPTY;

        switch (c) {
            case 'p':
                piece = BP;
                break;
            case 'r':
                piece = BR;
                break;
            case 'n':
                piece = BN;
                break;
            case 'b':
                piece = BB;
                break;
            case 'q':
                piece = BQ;
                break;
            case 'k':
                piece = BK;
                break;
            case 'P':
                piece = WP;
                break;
            case 'R':
                piece = WR;
                break;
            case 'N':
                piece = WN;
                break;
            case 'B':
                piece = WB;
                break;
            case 'Q':
                piece = WQ;
                break;
            case 'K':
                piece = WK;
                break;
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
                piece = EMPTY;
                count = c - '0';
                break;
            case '/':
            case ' ':
                rank--;
                file = FILE_A;
                index++;
                continue;
            default:
                return false;
        }

        for (int i = 0; i < count; i++) {
            sq64 = rank * 8 + file;
            if (sq64 >= 0 && sq64 < 64) {
                sq120 = SQ120[sq64];
                if (piece != EMPTY) {
                    board[sq120] = piece;
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

    side = fen[index] == 'w' ? WHITE : BLACK;
    index += 2;

    for (int i = 0; i < 4; i++) {
        if (index >= fen.length() || fen[index] == ' ') {
            break;
        }
        switch (fen[index]) {
            case 'K':
                castlePerm |= WKCA;
                break;
            case 'Q':
                castlePerm |= WQCA;
                break;
            case 'k':
                castlePerm |= BKCA;
                break;
            case 'q':
                castlePerm |= BQCA;
                break;
        }
        index++;
    }
    index++;

    if (index < fen.length() && fen[index] != '-') {
        if (index + 1 < fen.length()) { // Check bounds for valid EnPas parsing
            file = fen[index] - 'a';
            rank = fen[index + 1] - '1';
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
        // Check if multiplication would overflow beyond 1000
        if (fiftyMove > 1000 || (fiftyMove == 100 && digit > 0) || fiftyMove * 10 + digit > 1000) {
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
    if (side == WHITE) {
        std::cout << "     white" << std::endl;
    }
    else {
        std::cout << "     black" << std::endl;
    }
    for (int rank = RANK_8; rank >= RANK_1; rank--) {
        for (int file = FILE_A; file <= FILE_H; file++) {
            square = FR2SQ(file, rank);
            piece = board[square];
            std::cout << PIECE_CHARS[piece] << " ";
        }
        std::cout << std::endl;
    }
}

bool board::Board::sqAttacked(const int square, const int side_in) const
{
    int targetSquare;
    int piece;

    // Pawn attacks
    if (side_in == WHITE) {
        if (board[square - 11] == WP || board[square - 9] == WP)
            return true;
    }
    else {
        if (board[square + 11] == BP || board[square + 9] == BP)
            return true;
    }

    // Knight attacks
    for (int i = 0; i < 8; i++) {
        piece = board[square + MOVE_DIR[WN][i]];
        if (side_in == WHITE && piece == WN) {
            return true;
        }
        if (side_in == BLACK && piece == BN) {
            return true;
        }
    }

    // Rook / Queen attacks
    for (int i = 0; i < 4; i++) {
        targetSquare = square + MOVE_DIR[WR][i];
        piece = board[targetSquare];
        while (piece != NO_SQ) {
            if (piece != EMPTY) {
                if (side_in == WHITE && (piece == WR || piece == WQ)) {
                    return true;
                }
                if (side_in == BLACK && (piece == BR || piece == BQ)) {
                    return true;
                }
                break;
            }
            targetSquare += MOVE_DIR[WR][i];
            piece = board[targetSquare];
        }
    }

    // Bishop / Queen attacks
    for (int i = 0; i < 4; i++) {
        targetSquare = square + MOVE_DIR[WB][i];
        piece = board[targetSquare];
        while (piece != NO_SQ) {
            if (piece != EMPTY) {
                if (side_in == WHITE && (piece == WB || piece == WQ)) {
                    return true;
                }
                if (side_in == BLACK && (piece == BB || piece == BQ)) {
                    return true;
                }
                break;
            }
            targetSquare += MOVE_DIR[WB][i];
            piece = board[targetSquare];
        }
    }

    // King attacks
    for (int i = 0; i < 8; i++) {
        piece = board[square + MOVE_DIR[WK][i]];
        if ((side_in == WHITE && piece == WK) || (side_in == BLACK && piece == BK)) {
            return true;
        }
    }
    return false;
}

bool board::Board::updateListMaterial()
{
    int piece;
    for (int i = 0; i < 120; i++) {
        piece = board[i];
        if (piece != NO_SQ && piece != EMPTY) {
            // Check bounds before adding to piece list
            if (pNum[piece] >= 10) {
                return false; // Return false to indicate overflow
            }
            pList[piece][pNum[piece]] = i;
            pNum[piece]++;
            material[PIECE_COLOR[piece]] += PIECE_VAL[piece];
            if (piece == WP) {
                board::bitboard::setBit(pawns[WHITE], SQ64[i]);
                board::bitboard::setBit(pawns[BOTH], SQ64[i]);
            }
            else if (piece == BP) {
                board::bitboard::setBit(pawns[BLACK], SQ64[i]);
                board::bitboard::setBit(pawns[BOTH], SQ64[i]);
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
        piece = board[square];
        if (piece != NO_SQ && piece != EMPTY) {
            key ^= PIECE_KEYS[piece][square];
        }
    }
    if (enPas != NO_SQ) {
        key ^= PIECE_KEYS[EMPTY][enPas];
    }
    if (side == WHITE) {
        key ^= SIDE_KEY;
    }
    hashKey = key;
}
