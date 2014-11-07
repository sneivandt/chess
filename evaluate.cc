#include "evaluate.h"

int evaluatePosition(Board& pos)
{
    int square;
    int score = pos.getMaterial(WHITE) - pos.getMaterial(BLACK);
    for(int i = 0; i < pos.getPieceNum(WP); i++) {
        square = pos.getPieceList(WP)[i];
        score += PAWN_TABLE[SQ64[square]];
    }
    for(int i = 0; i < pos.getPieceNum(BP); i++) {
        square = pos.getPieceList(BP)[i];
        score -= PAWN_TABLE[MIRROR64[SQ64[square]]];
    }
    for(int i = 0; i < pos.getPieceNum(WN); i++) {
        square = pos.getPieceList(WN)[i];
        score += KNIGHT_TABLE[SQ64[square]];
    }
    for(int i = 0; i < pos.getPieceNum(BN); i++) {
        square = pos.getPieceList(BN)[i];
        score -= KNIGHT_TABLE[MIRROR64[SQ64[square]]];
    }
    for(int i = 0; i < pos.getPieceNum(WB); i++) {
        square = pos.getPieceList(WB)[i];
        score += BISHOP_TABLE[SQ64[square]];
    }
    for(int i = 0; i < pos.getPieceNum(BB); i++) {
        square = pos.getPieceList(BB)[i];
        score -= BISHOP_TABLE[MIRROR64[SQ64[square]]];
    }
    for(int i = 0; i < pos.getPieceNum(WR); i++) {
        square = pos.getPieceList(WR)[i];
        score += ROOK_TABLE[SQ64[square]];
    }
    for(int i = 0; i < pos.getPieceNum(BR); i++) {
        square = pos.getPieceList(BR)[i];
        score -= ROOK_TABLE[MIRROR64[SQ64[square]]];
    }
    if(pos.getSide() == BLACK) {
        score *= -1;
    }
    return score;
}

