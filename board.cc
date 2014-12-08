#include "board.h"

uint64_t PIECE_KEYS[13][120];
uint64_t CASTLE_KEYS[16];
uint64_t SIDE_KEY;
uint64_t FILE_MASK[8];
uint64_t RANK_MASK[8];
uint64_t PASSED_PAWN_MASK[2][64];
uint64_t ISOLATED_PAWN_MASK[64];

constexpr char Board::DEFAULT_FEN[];
constexpr char Board::PIECE_CHARS[];
constexpr int Board::PIECE_COLOR[];
constexpr int Board::PIECE_NO_TEAM[];
constexpr int Board::PIECE_VAL[];
constexpr int Board::RANKS[];
constexpr int Board::FILES[];
constexpr int Board::SQ64[];
constexpr int Board::SQ120[];
constexpr int Board::CASTLE_PERM_MASK[];
constexpr int Board::MOVE_DIR[13][8];

void Board::reset()
{
    for(int i = 0; i < 120; i++) {
        board[i] = NO_SQ;
    }
    for(int i = 0; i < 64; i++) {
        board[SQ120[i]] = EMPTY;
    }
    for(int i = 0; i < 3; i++) {
        pawns[i] = 0ULL;
    }
    for(int i = 0; i < 13; i++) {
        pNum[i] = 0;
        for(int j = 0; j < 10; j++) {
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

bool Board::parseFen(const std::string fen)
{
    reset();
    int piece = 0;
    int sq64 = 0;
    int sq120 = 0;
    int rank = RANK_8;
    int file = FILE_A;
    unsigned int index = 0;
    while(index < fen.size()) {
        for(index = 0; rank >= RANK_1; index++) {
            int count = 1;
            switch(fen[index]) {
                case 'p': piece = BP; break;
                case 'r': piece = BR; break;
                case 'n': piece = BN; break;
                case 'b': piece = BB; break;
                case 'q': piece = BQ; break;
                case 'k': piece = BK; break;
                case 'P': piece = WP; break;
                case 'R': piece = WR; break;
                case 'N': piece = WN; break;
                case 'B': piece = WB; break;
                case 'Q': piece = WQ; break;
                case 'K': piece = WK; break;
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                    piece = EMPTY;
                    count = fen[index] - '0';
                    break;
                case '/':
                case ' ':
                    rank--;
                    file = FILE_A;
                    count = 0;
                    break;
                default:
                    return false;
            }
            for(int i = 0; i < count; i++) {
                sq64 = rank * 8 + file;
                sq120 = SQ120[sq64];
                if(piece != EMPTY) {
                    board[sq120] = piece;
                }
                file++;
            }
        }
        side = fen[index] == 'w' ? WHITE: BLACK;
        index += 2;
        for(int i = 0; i < 4; i++) {
            if(fen.size() < index || fen[index] == ' ') {
                break;
            }
            switch(fen[index]) {
                case 'K': castlePerm |= WKCA; break;
                case 'Q': castlePerm |= WQCA; break;
                case 'k': castlePerm |= BKCA; break;
                case 'q': castlePerm |= BQCA; break;
            }
            index++;
        }
        index++;
        if(fen.size() > index + 1 && fen[index] != '-') {
            file = fen[index] - 'a';
            rank = fen[index + 1] - '1';
            enPas = FR2SQ(file, rank);
            index ++;
        }
        break;
    }
    updateListMaterial();
    generateHash();
    return true;
}

void Board::print() const
{
    int square;
    int piece;
    std::cout << "     " << (side == WHITE ? "white": "black") << std::endl;
    for(int rank = RANK_8; rank >= RANK_1; rank--) {
        for(int file = FILE_A; file <= FILE_H; file++) {
            square = FR2SQ(file, rank);
            piece = board[square];
            std::cout << PIECE_CHARS[piece] << " ";
        }
        std::cout << std::endl;
    }
}

bool Board::sqAttacked(const int square, const int side) const
{
    int targetSquare;
    int piece;
    if(side == WHITE && (board[square - 11] == WP || board[square - 9] == WP)) {
        return true;
    }
    else if(side == BLACK && (board[square + 11] == BP || board[square + 9] == BP)) {
        return true;
    }
    for(int i = 0; i < 8; i++) {
        piece = board[square + MOVE_DIR[WN][i]];
        if(side == WHITE && piece == WN) {
            return true;
        }
        else if(side == BLACK && piece == BN) {
            return true;
        }
    }
    for(int i = 0; i < 4; i++) {
        targetSquare = square + MOVE_DIR[WR][i];
        piece = board[targetSquare];
        while(piece != NO_SQ) {
            if(piece != EMPTY) {
                if(side == WHITE && (board[targetSquare] == WR || board[targetSquare] == WQ)) {
                    return true;
                }
                else if(side == BLACK && (board[targetSquare] == BR || board[targetSquare] == BQ)) {
                    return true;
                }
                break;
            }
            targetSquare += MOVE_DIR[WR][i];
            piece = board[targetSquare];
        }
    }
    for(int i = 0; i < 4; i++) {
        targetSquare = square + MOVE_DIR[WB][i];
        piece = board[targetSquare];
        while(piece != NO_SQ) {
            if(piece != EMPTY) {
                if(side == WHITE && (board[targetSquare] == WB || board[targetSquare] == WQ)) {
                    return true;
                }
                else if(side == BLACK && (board[targetSquare] == BB || board[targetSquare] == BQ)) {
                    return true;
                }
                break;
            }
            targetSquare += MOVE_DIR[WB][i];
            piece = board[targetSquare];
        }
    }
    for(int i = 0; i < 8; i++) {
        piece = board[square + MOVE_DIR[WK][i]];
        if((side == WHITE && piece == WK) || (side == BLACK && piece == BK)) {
            return true;
        }
    }
    return false;
}

void Board::updateListMaterial()
{
    int piece;
    for(int i = 0; i < 120; i++) {
        piece = board[i];
        if(piece != NO_SQ && piece != EMPTY) {
            pList[piece][pNum[piece]] = i;
            pNum[piece]++;
            material[PIECE_COLOR[piece]] += PIECE_VAL[piece];
            if(piece == WP) {
                bitboard::setBit(pawns[WHITE], SQ64[i]);
                bitboard::setBit(pawns[BOTH], SQ64[i]);
            }
            else if(piece == BP) {
                bitboard::setBit(pawns[BLACK], SQ64[i]);
                bitboard::setBit(pawns[BOTH], SQ64[i]);
            }
        }
    }
}

void Board::generateHash()
{
    uint64_t key = 0;
    int piece;
    for(int square = 0; square < 120; square++) {
        piece = board[square];
        if(piece != NO_SQ && piece != EMPTY) {
            key ^= PIECE_KEYS[piece][square];
        }
    }
    if(enPas != NO_SQ) {
        key ^= PIECE_KEYS[EMPTY][enPas];
    }
    if(side == WHITE) {
        key ^= SIDE_KEY;
    }
    hashKey = key;
}
