#include "movegen.h"

int movegen::MVVLVA_SCORES[13][13];

inline void movegen::addQuietMove(const int move, MoveList &list, Board &pos)
{
    int score = 0;
    if(pos.getSearchKiller(0) == move) {
        score = 900000;
    } else if(pos.getSearchKiller(1) == move) {
        score = 800000;
    }
    else {
        score = pos.getSearchHistory(pos.getSquare(Move::FROMSQ(move)), Move::TOSQ(move));
    }
    list.addMove(Move(move, score));
}

inline void movegen::addCaptureMove(const int move, MoveList &list, Board &pos)
{
    list.addMove(Move(move, MVVLVA_SCORES[Move::CAPTURED(move)][pos.getSquare(Move::FROMSQ(move))] + 1000000));
}

inline void movegen::addEnPasMove(const int move, MoveList &list)
{
    list.addMove(Move(move, MVVLVA_SCORES[WP][WP] + 1000000));
}

inline void movegen::addPawnMove(const int from, const int to, const int cap, const int side, MoveList &list, Board &pos)
{
    if(Board::RANKS[Board::SQ64[from]] == PAWN_RANK[side]) {
        for(int i = 0; i < 4; i++) {
            addCaptureMove(Move::MOVE(from, to, cap, PROMOTION_PIECES[side][i], 0), list, pos);
        }
    }
    else {
        addCaptureMove(Move::MOVE(from, to, cap, EMPTY, 0), list, pos);
    }
}

MoveList movegen::generateAll(Board &pos, const bool onlyCaps)
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
        if(!onlyCaps) {
            if(pos.getSquare(square + offset * 10) == EMPTY) {
                addPawnMove(square, square + offset * 10, EMPTY, pos.getSide(), moves, pos);
                if(Board::RANKS[Board::SQ64[square]] == PAWN_RANK[(pos.getSide() ^ 1)] && pos.getSquare(square + offset * 20) == EMPTY) {
                    addQuietMove(Move::MOVE(square, square + offset * 20, EMPTY, EMPTY, Move::MFLAGPS), moves, pos);
                }
            }
        }
        if(Board::SQ64[square + offset * 9] != NO_SQ && Board::PIECE_COLOR[pos.getSquare(square + offset * 9)] == (pos.getSide() ^ 1)) {
            addPawnMove(square, square + offset * 9, pos.getSquare(square + offset * 9), pos.getSide(), moves, pos);
        }
        if(Board::SQ64[square + offset * 11] != NO_SQ && Board::PIECE_COLOR[pos.getSquare(square + offset * 11)] == (pos.getSide() ^ 1)) {
            addPawnMove(square, square + offset * 11, pos.getSquare(square + offset * 11), pos.getSide(), moves, pos);
        }
        if(pos.getEnPas() != NO_SQ) {
            if(square + offset * 9 == pos.getEnPas()) {
                addEnPasMove(Move::MOVE(square, square + offset * 9, EMPTY, EMPTY, Move::MFLAGEP), moves);
            }
            if(square + offset * 11 == pos.getEnPas()) {
                addEnPasMove(Move::MOVE(square, square + offset * 11, EMPTY, EMPTY, Move::MFLAGEP), moves);
            }
        }
    }
    pos.getSide() == WHITE ? offset = 0 : offset = 3;
    for(int index = 0; index < 3; index++) {
        piece = SLIDERS[index + offset];
        for(int pceNum = 0; pceNum < pos.getPieceNum(piece); pceNum++) {
            square = pos.getPieceList(piece)[pceNum];
            for(int i = 0; i < 8 && Board::MOVE_DIR[piece][i] != 0; i++) {
                direction = Board::MOVE_DIR[piece][i];
                targetSquare = square + direction;
                while(pos.getSquare(targetSquare) != NO_SQ) {
                    if(pos.getSquare(targetSquare) != EMPTY) {
                        if(Board::PIECE_COLOR[pos.getSquare(targetSquare)] == (pos.getSide() ^ 1)) {
                            addCaptureMove(Move::MOVE(square, targetSquare, pos.getSquare(targetSquare), EMPTY, 0), moves, pos);
                        }
                        break;
                    }
                    if(!onlyCaps) {
                        addQuietMove(Move::MOVE(square, targetSquare, EMPTY, EMPTY, 0), moves, pos);
                    }
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
            for(int i = 0; i < 8 && Board::MOVE_DIR[piece][i] != 0; i++) {
                direction = Board::MOVE_DIR[piece][i];
                targetSquare = square + direction;
                if(pos.getSquare(targetSquare) == NO_SQ) {
                    continue;
                }
                if(pos.getSquare(targetSquare) != EMPTY) {
                    if(Board::PIECE_COLOR[pos.getSquare(targetSquare)] == (pos.getSide() ^ 1)) {
                        addCaptureMove(Move::MOVE(square, targetSquare, pos.getSquare(targetSquare), EMPTY, 0), moves, pos);
                    }
                    continue;
                }
                if(!onlyCaps) {
                    addQuietMove(Move::MOVE(square, targetSquare, EMPTY, EMPTY, 0), moves, pos);
                }
            }
        }
    }
    if(!onlyCaps) {
        if(pos.getSide() == WHITE) {
            if(pos.getCastlePerm() & WKCA) {
                if(pos.getSquare(F1) == EMPTY && pos.getSquare(G1) == EMPTY) {
                    if(!pos.sqAttacked(E1, BLACK) && !pos.sqAttacked(F1, BLACK)) {
                        addQuietMove(Move::MOVE(E1, G1, EMPTY, EMPTY, Move::MFLAGCA), moves, pos);
                    }
                }
            }
            if(pos.getCastlePerm() & WQCA) {
                if(pos.getSquare(D1) == EMPTY && pos.getSquare(C1) == EMPTY && pos.getSquare(B1) == EMPTY) {
                    if(!pos.sqAttacked(E1, BLACK) && !pos.sqAttacked(D1, BLACK)) {
                        addQuietMove(Move::MOVE(E1, C1, EMPTY, EMPTY, Move::MFLAGCA), moves, pos);
                    }
                }
            }
        }
        else {
            if(pos.getCastlePerm() & BKCA) {
                if(pos.getSquare(F8) == EMPTY && pos.getSquare(G8) == EMPTY) {
                    if(!pos.sqAttacked(E8, WHITE) && !pos.sqAttacked(F8, WHITE)) {
                        addQuietMove(Move::MOVE(E8, G8, EMPTY, EMPTY, Move::MFLAGCA), moves, pos);
                    }
                }
            }
            if(pos.getCastlePerm() & BQCA) {
                if(pos.getSquare(D8) == EMPTY && pos.getSquare(C8) == EMPTY && pos.getSquare(B8) == EMPTY) {
                    if(!pos.sqAttacked(E8, WHITE) && !pos.sqAttacked(D8, WHITE)) {
                        addQuietMove(Move::MOVE(E8, C8, EMPTY, EMPTY, Move::MFLAGCA), moves, pos);
                    }
                }
            }
        }
    }
    return moves;
}
