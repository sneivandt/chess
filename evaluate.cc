#include "evaluate.h"

int evaluate::evaluatePosition(Board &pos)
{
    int square;
    int score = pos.getMaterial(WHITE) - pos.getMaterial(BLACK);
    for(int piece = WP; piece <= BK; piece++) {
        for(int i = 0; i < pos.getPieceNum(piece); i++) {
            square = pos.getPieceList(piece)[i];
            if(PIECE_COLOR[piece] == WHITE) {
                score += PIECE_SQUARE_TABLE[PIECE_NO_TEAM[piece]][SQ64[square]];
            }
            else {
                score -= PIECE_SQUARE_TABLE[PIECE_NO_TEAM[piece]][MIRROR_PIECE_SQUARE_TABLE[SQ64[square]]];
            }
        }
    }
    if(pos.getSide() == BLACK) {
        score = -score;
    }
    return score;
}

