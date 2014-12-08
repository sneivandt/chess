#include "evaluate.h"

inline bool evaluate::materialDraw(Board &pos)
{
    if(!pos.getPieceNum(WR) && !pos.getPieceNum(BR) && !pos.getPieceNum(WQ) && !pos.getPieceNum(BQ) && !pos.getPieceNum(WP) && !pos.getPieceNum(BP)) {
        if(!pos.getPieceNum(BB) && !pos.getPieceNum(WB)) {
            if(pos.getPieceNum(WN) < 3 && pos.getPieceNum(BN) < 3) {
                return true;
            }
        }
        else if(!pos.getPieceNum(WN) && !pos.getPieceNum(BN)) {
            if(abs(pos.getPieceNum(WB) - pos.getPieceNum(BB)) < 2) {
                return true;
            }
        }
    }
    return false;
}

inline bool evaluate::isEndGame(Board &pos, const int side)
{
    if(side == WHITE) {
        if(!pos.getPieceNum(BQ) && pos.getMaterial(BLACK) < ENDGAME_MATERIAL) {
            return true;
        }
    }
    else {
        if(!pos.getPieceNum(WQ) && pos.getMaterial(WHITE) < ENDGAME_MATERIAL) {
            return true;
        }
    }
    return false;
}

int evaluate::score(Board &pos)
{
    if(materialDraw(pos)) {
        return 0;
    }
    int square;
    int side;
    int score = pos.getMaterial(WHITE) - pos.getMaterial(BLACK);
    if(pos.getPieceNum(WB) == 2) {
        score += BISHOP_PAIR;
    }
    if(pos.getPieceNum(BB) == 2) {
        score -= BISHOP_PAIR;
    }
    for(int piece = WP; piece <= BK; piece++) {
        for(int i = 0; i < pos.getPieceNum(piece); i++) {
            square = pos.getPieceList(piece)[i];
            side = Board::PIECE_COLOR[piece];
            if(Board::PIECE_COLOR[piece] == WHITE) {
                score += PIECE_SQUARE_TABLE[Board::PIECE_NO_TEAM[piece]][Board::SQ64[square]];
            }
            else {
                score -= PIECE_SQUARE_TABLE[Board::PIECE_NO_TEAM[piece]][MIRROR_PIECE_SQUARE_TABLE[Board::SQ64[square]]];
            }
            if(piece == WK) {
                if(isEndGame(pos, WHITE)) {
                    score += PIECE_SQUARE_TABLE_KING_END[Board::SQ64[square]];
                }
                else {
                    score += PIECE_SQUARE_TABLE_KING_OPENING[Board::SQ64[square]];
                }
            }
            else if(piece == BK) {
                if(isEndGame(pos, BLACK)) {
                    score -= PIECE_SQUARE_TABLE_KING_END[MIRROR_PIECE_SQUARE_TABLE[Board::SQ64[square]]];
                }
                else {
                    score -= PIECE_SQUARE_TABLE_KING_OPENING[MIRROR_PIECE_SQUARE_TABLE[Board::SQ64[square]]];
                }
            }
            else if(Board::PIECE_NO_TEAM[piece] == WP) {
                if(!pos.getPawns()[side] & ISOLATED_PAWN_MASK[Board::SQ64[square]]) {
                    score += PAWN_ISOLATED * (side == WHITE ? 1 : -1);
                }
                if(!pos.getPawns()[side ^ 1] & PASSED_PAWN_MASK[side][Board::SQ64[square]]) {
                    score += PASSED_PAWN[Board::RANKS[square]] * (side == WHITE ? 1 : -1);
                }
            }
            else if(Board::PIECE_NO_TEAM[piece] == WR) {
                if(!pos.getPawns()[BOTH] & FILE_MASK[Board::FILES[square]]) {
                    score += ROOK_OPEN_FILE * (side == WHITE ? 1 : -1);
                }
                else if(!pos.getPawns()[side] & FILE_MASK[Board::FILES[square]]) {
                    score += ROOK_SEMI_OPEN_FILE * (side == WHITE ? 1 : -1);
                }
            }
            else if(Board::PIECE_NO_TEAM[piece] == WQ) {
                if(!pos.getPawns()[BOTH] & FILE_MASK[Board::FILES[square]]) {
                    score += QUEEN_OPEN_FILE * (side == WHITE ? 1 : -1);
                }
                else if(!pos.getPawns()[side] & FILE_MASK[Board::FILES[square]]) {
                    score += QUEEN_SEMI_OPEN_FILE * (side == WHITE ? 1 : -1);
                }
            }
        }
    }
    if(pos.getSide() == BLACK) {
        score = -score;
    }
    return score;
}
