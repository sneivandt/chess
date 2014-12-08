#include "makemove.h"

inline void makemove::clearPiece(const int square, Board &pos)
{
    int piece = pos.getSquare(square);
    int color = Board::PIECE_COLOR[piece];
    int targetPNum = -1;
    pos.addMaterial(color, -1 * Board::PIECE_VAL[piece]);
    pos.hashPiece(piece, square);
    pos.setSquare(square, EMPTY);
    if(Board::PIECE_NO_TEAM[piece] == WP) {
        bitboard::clearBit(pos.getPawns()[color], Board::SQ64[square]);
        bitboard::clearBit(pos.getPawns()[BOTH], Board::SQ64[square]);
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

inline void makemove::addPiece(const int piece, const int square, Board &pos)
{
    int color = Board::PIECE_COLOR[piece];
    pos.addMaterial(color, Board::PIECE_VAL[piece]);
    pos.setSquare(square, piece);
    pos.hashPiece(piece, square);
    if(Board::PIECE_NO_TEAM[piece] == WP) {
        bitboard::setBit(pos.getPawns()[color], Board::SQ64[square]);
        bitboard::setBit(pos.getPawns()[BOTH], Board::SQ64[square]);
    }
    pos.getPieceList(piece)[pos.getPieceNum(piece)] = square;
    pos.incrementPieceNum(piece);
}

inline void makemove::movePiece(const int from, const int to, Board &pos)
{
    int piece = pos.getSquare(from);
    int color = Board::PIECE_COLOR[piece];
    pos.hashPiece(piece, to);
    pos.hashPiece(piece, from);
    pos.setSquare(from, EMPTY);
    pos.setSquare(to, piece);
    if(Board::PIECE_NO_TEAM[piece] == WP) {
        bitboard::clearBit(pos.getPawns()[color], Board::SQ64[from]);
        bitboard::clearBit(pos.getPawns()[BOTH], Board::SQ64[from]);
        bitboard::setBit(pos.getPawns()[color], Board::SQ64[to]);
        bitboard::setBit(pos.getPawns()[BOTH], Board::SQ64[to]);
    }
    for(int i = 0; i < pos.getPieceNum(piece); i++) {
        if(pos.getPieceList(piece)[i] == from) {
            pos.getPieceList(piece)[i] = to;
            break;
        }
    }
}

bool makemove::move(Move &move, Board &pos)
{
    int from = Move::FROMSQ(move.getValue());
    int to = Move::TOSQ(move.getValue());
    int captured = Move::CAPTURED(move.getValue());
    int promoted = Move::PROMOTED(move.getValue());
    int side = pos.getSide();
    int piece = pos.getSquare(from);
    int castle = pos.getCastlePerm();
    int enPas = pos.getEnPas();
    int fiftyMove = pos.getFiftyMove();
    uint64_t hash = pos.getHashKey();
    Undo current = Undo(castle, enPas, fiftyMove, move.getValue(), hash);
    if(move.getValue() & Move::MFLAGEP) {
        if(side == WHITE) {
            clearPiece(to - 10, pos);
        }
        else {
            clearPiece(to + 10, pos);
        }
    }
    else if(move.getValue() & Move::MFLAGCA) {
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
    if(Board::PIECE_NO_TEAM[piece] == WP) {
        pos.resetFiftyMove();
        if(move.getValue() & Move::MFLAGPS) {
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
        undo(pos);
        return false;
    }
    else if(pos.getSide() == WHITE && pos.sqAttacked(pos.getPieceList(BK)[0], WHITE)) {
        undo(pos);
        return false;
    }
    return true;
}

void makemove::undo(Board &pos)
{
    pos.decrementPly();
    Undo undo = pos.popHistory();
    int move = undo.getMoveValue();
    int from = Move::FROMSQ(move);
    int to = Move::TOSQ(move);
    int captured = Move::CAPTURED(move);
    int promoted = Move::PROMOTED(move);
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
    if(Move::MFLAGEP & move) {
        if(pos.getSide() == WHITE) {
            addPiece(BP, to - 10, pos);
        }
        else {
            addPiece(WP, to + 10, pos);
        }
    }
    else if(Move::MFLAGCA & move) {
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
        addPiece(Board::PIECE_COLOR[promoted] == WHITE ? WP : BP, from, pos);
    }
}
