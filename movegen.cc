#include "movegen.h"

inline void addQuietMove(const int move, MoveList &list)
{
    list.addMove(Move(move, 0));
}

inline void addCaptureMove(const int move, MoveList &list)
{
    list.addMove(Move(move, 0));
}

inline void addEnPasMove(const int move, MoveList &list)
{
    list.addMove(Move(move, 0));
}

inline void addPawnMove(const int from, const int to, const int side, MoveList &list)
{
    if(RANKS[SQ64[from]] == PAWN_RANK[side]) {
        for(int i = 0; i < 4; i++) {
            addQuietMove(MOVE(from, to, 0, PROMOTION_PIECES[side][i], 0), list);
        }
    }
    else {
        addQuietMove(MOVE(from, to, 0, EMPTY, 0), list);
    }
}

inline void addPawnMove(const int from, const int to, const int cap, const int side, MoveList &list)
{
    if(RANKS[SQ64[from]] == PAWN_RANK[side]) {
        for(int i = 0; i < 4; i++) {
            addCaptureMove(MOVE(from, to, cap, PROMOTION_PIECES[side][i], 0), list);
        }
    }
    else {
        addCaptureMove(MOVE(from, to, cap, EMPTY, 0), list);
    }
}

MoveList generateAllMoves(Board &pos)
{
    MoveList moves;
    int square;
    int piece;
    int offset;
    int direction;
    int targetSquare;
    pos.getSide() == WHITE ? piece = WP : piece = BP;
    pos.getSide() == WHITE ? offset = 1 : offset = -1;
    for(int pceNum = 0; pceNum < pos.getPieceNum(piece); pceNum++) {
        square = pos.getPieceList(piece)[pceNum];
        if(pos.getSquare(square + offset * 10) == EMPTY) {
            addPawnMove(square, square + offset * 10, pos.getSide(), moves);
            if(RANKS[SQ64[square]] == PAWN_RANK[(pos.getSide() ^ 1)] && pos.getSquare(square + offset * 20) == EMPTY) {
                addQuietMove(MOVE(square, square + offset * 20, EMPTY, EMPTY, MFLAGPS), moves);
            }
        }
        if(SQ64[square + offset * 9] != NO_SQ && PIECE_COLOR[pos.getSquare(square + offset * 9)] == (pos.getSide() ^ 1)) {
            addPawnMove(square, square + offset * 9, pos.getSquare(square + offset * 9), pos.getSide(), moves);
        }
        if(SQ64[square + offset * 11] != NO_SQ && PIECE_COLOR[pos.getSquare(square + offset * 11)] == (pos.getSide() ^ 1)) {
            addPawnMove(square, square + offset * 11, pos.getSquare(square + offset * 11), pos.getSide(), moves);
        }
        if(square + offset * 9 == pos.getEnPas()) {
            addEnPasMove(MOVE(square, square + offset * 9, EMPTY, EMPTY, MFLAGEP), moves);
        }
        if(square + offset * 11 == pos.getEnPas()) {
            addEnPasMove(MOVE(square, square + offset * 11, EMPTY, EMPTY, MFLAGEP), moves);
        }
    }
    pos.getSide() == WHITE ? offset = 0 : offset = 3;
    for(int index = 0; index < 3; index++) {
        piece = SLIDERS[index + offset];
        for(int pceNum = 0; pceNum < pos.getPieceNum(piece); pceNum++) {
            square = pos.getPieceList(piece)[pceNum];
            for(int i = 0; i < 8 && MOVE_DIR[piece][i] != 0; i++) {
                direction = MOVE_DIR[piece][i];
                targetSquare = square + direction;
                while(pos.getSquare(targetSquare) != NO_SQ) {
                    if(pos.getSquare(targetSquare) != EMPTY) {
                        if(PIECE_COLOR[pos.getSquare(targetSquare)] == (pos.getSide() ^ 1)) {
                            addCaptureMove(MOVE(square, targetSquare, pos.getSquare(targetSquare), EMPTY, 0), moves);
                        }
                        break;
                    }
                    addQuietMove(MOVE(square, targetSquare, EMPTY, EMPTY, 0), moves);
                    targetSquare += direction;
                }
            }
        }
    }
    pos.getSide() == WHITE ? offset = 0 : offset = 2;
    for(int index = 0; index < 2; index++) {
        piece = NON_SLIDERS[index + offset];
        for(int pceNum = 0; pceNum < pos.getPieceNum(piece); pceNum++) {
            square = pos.getPieceList(piece)[pceNum];
            for(int i = 0; i < 8 && MOVE_DIR[piece][i] != 0; i++) {
                direction = MOVE_DIR[piece][i];
                targetSquare = square + direction;
                if(pos.getSquare(targetSquare) == NO_SQ) {
                    continue;
                }
                if(pos.getSquare(targetSquare) != EMPTY) {
                    if(PIECE_COLOR[pos.getSquare(targetSquare)] == (pos.getSide() ^ 1)) {
                        addCaptureMove(MOVE(square, targetSquare, pos.getSquare(targetSquare), EMPTY, 0), moves);
                    }
                    continue;
                }
                addQuietMove(MOVE(square, targetSquare, EMPTY, EMPTY, 0), moves);
            }
        }
    }
    if(pos.getSide() == WHITE) {
        if(pos.getCastlePerm() & WKCA) {
            if(pos.getSquare(F1) == EMPTY && pos.getSquare(G1) == EMPTY) {
                if(!pos.sqAttacked(E1, BLACK) && !pos.sqAttacked(F1, BLACK)) {
                    addQuietMove(MOVE(E1, G1, EMPTY, EMPTY, MFLAGCA), moves);
                }
            }
        }
        if(pos.getCastlePerm() & WQCA) {
            if(pos.getSquare(D1) == EMPTY && pos.getSquare(C1) == EMPTY && pos.getSquare(B1) == EMPTY) {
                if(!pos.sqAttacked(E1, BLACK) && !pos.sqAttacked(D1, BLACK)) {
                    addQuietMove(MOVE(E1, C1, EMPTY, EMPTY, MFLAGCA), moves);
                }
            }
        }
    }
    else {
        if(pos.getCastlePerm() & BKCA) {
            if(pos.getSquare(F8) == EMPTY && pos.getSquare(G8) == EMPTY) {
                if(!pos.sqAttacked(E8, WHITE) && !pos.sqAttacked(F8, WHITE)) {
                    addQuietMove(MOVE(E8, G8, EMPTY, EMPTY, MFLAGCA), moves);
                }
            }
        }
        if(pos.getCastlePerm() & BQCA) {
            if(pos.getSquare(D8) == EMPTY && pos.getSquare(C8) == EMPTY && pos.getSquare(B8) == EMPTY) {
                if(!pos.sqAttacked(E8, WHITE) && !pos.sqAttacked(D8, WHITE)) {
                    addQuietMove(MOVE(E8, C8, EMPTY, EMPTY, MFLAGCA), moves);
                }
            }
        }
    }
    return moves;
}
