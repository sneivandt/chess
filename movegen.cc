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

inline void addWhitePawnMove(const int from, const int to, MoveList &list)
{
    if(RANKS[SQ64[from]] == RANK_7) {
        addQuietMove(MOVE(from, to, 0, WQ, 0), list);
        addQuietMove(MOVE(from, to, 0, WR, 0), list);
        addQuietMove(MOVE(from, to, 0, WB, 0), list);
        addQuietMove(MOVE(from, to, 0, WN, 0), list);
    }
    else {
        addQuietMove(MOVE(from, to, 0, EMPTY, 0), list);
    }
}

inline void addWhitePawnCaptureMove(const int from, const int to, const int cap, MoveList &list)
{
    if(RANKS[SQ64[from]] == RANK_7) {
        addCaptureMove(MOVE(from, to, cap, WQ, 0), list);
        addCaptureMove(MOVE(from, to, cap, WR, 0), list);
        addCaptureMove(MOVE(from, to, cap, WB, 0), list);
        addCaptureMove(MOVE(from, to, cap, WN, 0), list);
    }
    else {
        addCaptureMove(MOVE(from, to, cap, EMPTY, 0), list);
    }
}

inline void addBlackPawnMove(const int from, const int to, MoveList &list)
{
    if(RANKS[SQ64[from]] == RANK_2) {
        addQuietMove(MOVE(from, to, 0, BQ, 0), list);
        addQuietMove(MOVE(from, to, 0, BR, 0), list);
        addQuietMove(MOVE(from, to, 0, BB, 0), list);
        addQuietMove(MOVE(from, to, 0, BN, 0), list);
    }
    else {
        addQuietMove(MOVE(from, to, 0, EMPTY, 0), list);
    }
}

inline void addBlackPawnCaptureMove(const int from, const int to, const int cap, MoveList &list)
{
    if(RANKS[SQ64[from]] == RANK_2) {
        addCaptureMove(MOVE(from, to, cap, BQ, 0), list);
        addCaptureMove(MOVE(from, to, cap, BR, 0), list);
        addCaptureMove(MOVE(from, to, cap, BB, 0), list);
        addCaptureMove(MOVE(from, to, cap, BN, 0), list);
    }
    else {
        addCaptureMove(MOVE(from, to, cap, EMPTY, 0), list);
    }
}

MoveList generateAllMoves(Board &pos)
{
    MoveList moves;
    int square = 0;
    int piece = 0;
    int offset = 0;
    int direction = 0;
    int targetSquare = 0;
    if(pos.getSide() == WHITE) {
        for(int pceNum = 0; pceNum < pos.getPieceNum(WP); pceNum++) {
            square = pos.getPieceList(WP)[pceNum];
            if(pos.getBoard()[square + 10] == EMPTY) {
                addWhitePawnMove(square, square + 10, moves);
                if(RANKS[SQ64[square]] == RANK_2 && pos.getBoard()[square + 20] == EMPTY) {
                    addQuietMove(MOVE(square, square + 20, EMPTY, EMPTY, MFLAGPS), moves);
                }
            }
            if(SQ64[square + 9] != NO_SQ && PIECE_COLOR[pos.getBoard()[square + 9]] == BLACK) {
                addWhitePawnCaptureMove(square, square + 9, pos.getBoard()[square + 9], moves);
            }
            if(SQ64[square + 11] != NO_SQ && PIECE_COLOR[pos.getBoard()[square + 11]] == BLACK) {
                addWhitePawnCaptureMove(square, square + 11, pos.getBoard()[square + 11], moves);
            }
            if(square + 9 == pos.getEnPas()) {
                addEnPasMove(MOVE(square, square + 9, EMPTY, EMPTY, MFLAGEP), moves);
            }
            if(square + 11 == pos.getEnPas()) {
                addEnPasMove(MOVE(square, square + 11, EMPTY, EMPTY, MFLAGEP), moves);
            }
        }
        if(pos.getCastlePerm() & WKCA) {
            if(pos.getBoard()[F1] == EMPTY && pos.getBoard()[G1] == EMPTY) {
                if(!pos.sqAttacked(E1, BLACK) && !pos.sqAttacked(F1, BLACK)) {
                    addQuietMove(MOVE(E1, G1, EMPTY, EMPTY, MFLAGCA), moves);
                }
            }
        }
        if(pos.getCastlePerm() & WQCA) {
            if(pos.getBoard()[D1] == EMPTY && pos.getBoard()[C1] == EMPTY && pos.getBoard()[B1] == EMPTY) {
                if(!pos.sqAttacked(E1, BLACK) && !pos.sqAttacked(D1, BLACK)) {
                    addQuietMove(MOVE(E1, C1, EMPTY, EMPTY, MFLAGCA), moves);
                }
            }
        }
    }
    else {
        for(int pceNum = 0; pceNum < pos.getPieceNum(BP); pceNum++) {
            square = pos.getPieceList(BP)[pceNum];
            if(pos.getBoard()[square - 10] == EMPTY) {
                addBlackPawnMove(square, square - 10, moves);
                if(RANKS[SQ64[square]] == RANK_7 && pos.getBoard()[square - 20] == EMPTY) {
                    addQuietMove(MOVE(square, square - 20, EMPTY, EMPTY, MFLAGPS), moves);
                }
            }
            if(SQ64[square - 9] != NO_SQ && PIECE_COLOR[pos.getBoard()[square - 9]] == WHITE) {
                addBlackPawnCaptureMove(square, square - 9, pos.getBoard()[square - 9], moves);
            }
            if(SQ64[square - 11] != NO_SQ && PIECE_COLOR[pos.getBoard()[square - 11]] == WHITE) {
                addBlackPawnCaptureMove(square, square - 11, pos.getBoard()[square - 11], moves);
            }
            if(square - 9 == pos.getEnPas()) {
                addEnPasMove(MOVE(square, square - 9, EMPTY, EMPTY, MFLAGEP), moves);
            }
            if(square - 11 == pos.getEnPas()) {
                addEnPasMove(MOVE(square, square - 11, EMPTY, EMPTY, MFLAGEP), moves);
            }
        }
        if(pos.getCastlePerm() & BKCA) {
            if(pos.getBoard()[F8] == EMPTY && pos.getBoard()[G8] == EMPTY) {
                if(!pos.sqAttacked(E8, WHITE) && !pos.sqAttacked(F8, WHITE)) {
                    addQuietMove(MOVE(E8, G8, EMPTY, EMPTY, MFLAGCA), moves);
                }
            }
        }
        if(pos.getCastlePerm() & BQCA) {
            if(pos.getBoard()[D8] == EMPTY && pos.getBoard()[C8] == EMPTY && pos.getBoard()[B8] == EMPTY) {
                if(!pos.sqAttacked(E8, WHITE) && !pos.sqAttacked(D8, WHITE)) {
                    addQuietMove(MOVE(E8, C8, EMPTY, EMPTY, MFLAGCA), moves);
                }
            }
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
                while(pos.getBoard()[targetSquare] != NO_SQ) {
                    if(pos.getBoard()[targetSquare] != EMPTY) {
                        if(PIECE_COLOR[pos.getBoard()[targetSquare]] == (pos.getSide() ^ 1)) {
                            addCaptureMove(MOVE(square, targetSquare, pos.getBoard()[targetSquare], EMPTY, 0), moves);
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
                if(pos.getBoard()[targetSquare] == NO_SQ) {
                    continue;
                }
                if(pos.getBoard()[targetSquare] != EMPTY) {
                    if(PIECE_COLOR[pos.getBoard()[targetSquare]] == (pos.getSide() ^ 1)) {
                        addCaptureMove(MOVE(square, targetSquare, pos.getBoard()[targetSquare], EMPTY, 0), moves);
                    }
                    continue;
                }
                addQuietMove(MOVE(square, targetSquare, EMPTY, EMPTY, 0), moves);
            }
        }
    }
    return moves;
}
