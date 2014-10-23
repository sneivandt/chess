#include "makemove.h"

inline void clearPiece(const int square, Board& pos)
{
    int piece = pos.getBoard()[square];
    int color = PIECE_COLOR[piece];
    int targetPNum = -1;
    pos.hashPiece(piece, square);
    pos.getBoard()[square] = EMPTY;
    if(PIECE_PAWN[piece]) {
        clearBit(pos.getPawns()[color], SQ64[square]);
        clearBit(pos.getPawns()[BOTH], SQ64[square]);
    }
    for(int i = 0; i < pos.getPieceNum(piece); i++) {
        if(pos.getPieceList(piece)[i] == square) {
            targetPNum = i;
            break;
        }
    }
    pos.decrementPieceNum(piece);
    pos.getPieceList(piece)[targetPNum] = pos.getPieceList(piece)[pos.getPieceNum(piece)];
}

inline void addPiece(const int piece, const int square, Board& pos)
{
    int color = PIECE_COLOR[piece];
    pos.getBoard()[square] = piece;
    pos.hashPiece(piece, square);
    if(PIECE_PAWN[piece]) {
        setBit(pos.getPawns()[color], SQ64[square]);
        setBit(pos.getPawns()[BOTH], SQ64[square]);
    }
    pos.getPieceList(piece)[pos.getPieceNum(piece)] = square;
    pos.incrementPieceNum(piece);
}

inline void movePiece(const int from, const int to, Board& pos)
{
    int piece = pos.getBoard()[from];
    int color = PIECE_COLOR[piece];
    pos.hashPiece(piece, to);
    pos.hashPiece(piece, from);
    pos.getBoard()[from] = EMPTY;
    pos.getBoard()[to] = piece;
    if(PIECE_PAWN[piece]) {
        clearBit(pos.getPawns()[color], SQ64[from]);
        clearBit(pos.getPawns()[BOTH], SQ64[from]);
        setBit(pos.getPawns()[color], SQ64[to]);
        setBit(pos.getPawns()[BOTH], SQ64[to]);
    }
    for(int i = 0; i < pos.getPieceNum(piece); i++) {
        if(pos.getPieceList(piece)[i] == from) {
            pos.getPieceList(piece)[i] = to;
            break;
        }
    }
}

bool makeMove(Move& move, Board& pos)
{
    int from = FROMSQ(move.getValue());
    int to = TOSQ(move.getValue());
    int captured = CAPTURED(move.getValue());
    int promoted = PROMOTED(move.getValue());
    int side = pos.getSide();
    int piece = pos.getBoard()[from];
    int castle = pos.getCastlePerm();
    int enPas = pos.getEnPas();
    int fiftyMove = pos.getFiftyMove();
    uint64_t hash = pos.getHashKey();
    Undo current = Undo(side, castle, enPas, fiftyMove, move.getValue(), hash);
    if(move.getValue() & MFLAGEP) {
        if(side == WHITE) {
            clearPiece(to - 10, pos);
        }
        else {
            clearPiece(to + 10, pos);
        }
    }
    else if(move.getValue() & MFLAGCA) {
        if(to == C1) {
            movePiece(A1, D1, pos);
        }
        else if(to == C8) {
            movePiece(A8, D8, pos);
        }
        else if(to == G1) {
            movePiece(H1, F1, pos);
        }
        else if(to == G8) {
            movePiece(H8, F8, pos);
        }
    }
    if(pos.getEnPas() != NO_SQ) {
        pos.hashEnPas();
    }
    pos.hashCastle();
    pos.addHistory(current);
    pos.updateCastlePerm(to, from);
    pos.hashCastle();
    pos.clearEnPas();
    pos.incrementFiftyMove();
    if(captured != EMPTY) {
        clearPiece(to, pos);
        pos.resetFiftyMove();
    }
    pos.incrementPly();
    pos.incrementHistoryPly();
    if(PIECE_PAWN[piece]) {
        pos.resetFiftyMove();
        if(move.getValue() & MFLAGPS) {
            if(side == WHITE) {
                pos.setEnPas(from + 10);
            }
            else {
                pos.setEnPas(from - 10);
            }
            pos.hashEnPas();
        }
    }
    movePiece(from, to, pos);
    if(promoted != EMPTY) {
        clearPiece(to, pos);
        addPiece(promoted, to, pos);
    }
    pos.updateSide();
    pos.hashSide();
    if(pos.getSide() == BLACK && pos.sqAttacked(pos.getPieceList(WK)[0], BLACK)) {
        takeMove(pos);
        return false;
    }
    else if(pos.getSide() == WHITE && pos.sqAttacked(pos.getPieceList(BK)[0], WHITE)) {
        takeMove(pos);
        return false;
    }
    return true;
}

void takeMove(Board& pos)
{
    pos.decrementPly();
    pos.decrementHistoryPly();
    Undo undo = pos.getHistory()[pos.getHistoryPly()];
    int move = undo.getMoveValue();
    int from = FROMSQ(move);
    int to = TOSQ(move);
    int captured = CAPTURED(move);
    int promoted = PROMOTED(move);
    pos.setFiftyMove(undo.getFiftyMove());
    if(pos.getEnPas() != NO_SQ) {
        pos.hashEnPas();
    }
    pos.setEnPas(undo.getEnPas());
    if(pos.getEnPas() != NO_SQ) {
        pos.hashEnPas();
    }
    pos.hashCastle();
    pos.setCastlePerm(undo.getCastlePerm());
    pos.hashCastle();
    pos.updateSide();
    pos.hashSide();
    if(MFLAGEP & move) {
        if(pos.getSide() == WHITE) {
            addPiece(BP, to - 10, pos);
        }
        else {
            addPiece(WP, to + 10, pos);
        }
    }
    else if(MFLAGCA & move) {
        if(to == C1) {
            movePiece(D1, A1, pos);
        }
        else if(to == C8) {
            movePiece(D8, A8, pos);
        }
        else if(to == G1) {
            movePiece(F1, H1, pos);
        }
        else if(to == G8) {
            movePiece(F8, H8, pos);
        }
    }
    movePiece(to, from, pos);
    if(captured != EMPTY) {
        addPiece(captured, to, pos);
    }
    if(promoted != EMPTY) {
        clearPiece(from, pos);
        addPiece(PIECE_COLOR[promoted] == WHITE ? WP : BP, from, pos);
    }
}