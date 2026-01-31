#include "board/evaluate.h"

#include "board/board.h"

#include <cstdint>
#include <cstdlib>

inline bool board::evaluate::materialDraw(Board& pos)
{
    if (!pos.getPieceNum(WR) && !pos.getPieceNum(BR) && !pos.getPieceNum(WQ) && !pos.getPieceNum(BQ) &&
        !pos.getPieceNum(WP) && !pos.getPieceNum(BP)) {
        if (!pos.getPieceNum(BB) && !pos.getPieceNum(WB)) {
            if (pos.getPieceNum(WN) < 3 && pos.getPieceNum(BN) < 3) {
                return true;
            }
        }
        else if (!pos.getPieceNum(WN) && !pos.getPieceNum(BN)) {
            if (abs(pos.getPieceNum(WB) - pos.getPieceNum(BB)) < 2) {
                return true;
            }
        }
    }
    return false;
}

inline bool board::evaluate::isEndGame(Board& pos, const int side)
{
    if (side == WHITE) {
        if (!pos.getPieceNum(BQ) && pos.getMaterial(BLACK) < ENDGAME_MATERIAL) {
            return true;
        }
    }
    else {
        if (!pos.getPieceNum(WQ) && pos.getMaterial(WHITE) < ENDGAME_MATERIAL) {
            return true;
        }
    }
    return false;
}

int board::evaluate::score(Board& pos)
{
    if (materialDraw(pos)) {
        return 0;
    }
    int square;
    int side;
    int score = pos.getMaterial(WHITE) - pos.getMaterial(BLACK);
    if (pos.getPieceNum(WB) == 2) {
        score += BISHOP_PAIR;
    }
    if (pos.getPieceNum(BB) == 2) {
        score -= BISHOP_PAIR;
    }
    for (int piece = WP; piece <= BK; piece++) {
        for (int i = 0; i < pos.getPieceNum(piece); i++) {
            square = pos.getPieceList(piece)[i];
            side = Board::PIECE_COLOR[piece];
            const auto sq64 = static_cast<size_t>(Board::SQ64[square]);
            const auto pieceType = static_cast<size_t>(Board::PIECE_NO_TEAM[piece]);
            
            if (Board::PIECE_COLOR[piece] == WHITE) {
                score += PIECE_SQUARE_TABLE[pieceType][sq64];
            }
            else {
                const auto mirroredSq = static_cast<size_t>(MIRROR_PIECE_SQUARE_TABLE[sq64]);
                score -= PIECE_SQUARE_TABLE[pieceType][mirroredSq];
            }
            if (piece == WK) {
                if (isEndGame(pos, WHITE)) {
                    score += PIECE_SQUARE_TABLE_KING_END[sq64];
                }
                else {
                    score += PIECE_SQUARE_TABLE_KING_OPENING[sq64];
                }
            }
            else if (piece == BK) {
                const auto mirroredSq = static_cast<size_t>(MIRROR_PIECE_SQUARE_TABLE[sq64]);
                if (isEndGame(pos, BLACK)) {
                    score -= PIECE_SQUARE_TABLE_KING_END[mirroredSq];
                }
                else {
                    score -= PIECE_SQUARE_TABLE_KING_OPENING[mirroredSq];
                }
            }
            else if (Board::PIECE_NO_TEAM[piece] == WP) {
                if (!(pos.getPawns()[side] & Board::ISOLATED_PAWN_MASK[Board::SQ64[square]])) {
                    score += PAWN_ISOLATED * (side == WHITE ? 1 : -1);
                }
                if (!(pos.getPawns()[side ^ 1] & Board::PASSED_PAWN_MASK[side][Board::SQ64[square]])) {
                    const auto rank = static_cast<size_t>(Board::RANKS[Board::SQ64[square]]);
                    score += PASSED_PAWN[rank] * (side == WHITE ? 1 : -1);
                }
            }
            else if (Board::PIECE_NO_TEAM[piece] == WR) {
                if (!(pos.getPawns()[BOTH] & Board::FILE_MASK[Board::FILES[Board::SQ64[square]]])) {
                    score += ROOK_OPEN_FILE * (side == WHITE ? 1 : -1);
                }
                else if (!(pos.getPawns()[side] & Board::FILE_MASK[Board::FILES[Board::SQ64[square]]])) {
                    score += ROOK_SEMI_OPEN_FILE * (side == WHITE ? 1 : -1);
                }
            }
            else if (Board::PIECE_NO_TEAM[piece] == WQ) {
                if (!(pos.getPawns()[BOTH] & Board::FILE_MASK[Board::FILES[Board::SQ64[square]]])) {
                    score += QUEEN_OPEN_FILE * (side == WHITE ? 1 : -1);
                }
                else if (!(pos.getPawns()[side] & Board::FILE_MASK[Board::FILES[Board::SQ64[square]]])) {
                    score += QUEEN_SEMI_OPEN_FILE * (side == WHITE ? 1 : -1);
                }
            }
        }
    }
    if (pos.getSide() == BLACK) {
        score = -score;
    }
    return score;
}
