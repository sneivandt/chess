#include "board/makemove.h"

#include "bitboard.h"
#include "board/board.h"
#include "board/move.h"
#include "board/undo.h"

#include <cassert>
#include <cstdint>
#include <stdexcept>

inline void board::makemove::clearPiece(const int square, Board& pos)
{
    int piece = pos.getSquare(square);
    int color = Board::PIECE_COLOR[idx(piece)];
    int targetPNum = -1;
    pos.addMaterial(color, -1 * Board::PIECE_VAL[idx(piece)]);
    pos.hashPiece(piece, square);
    pos.setSquare(square, toInt(Piece::EMPTY));
    if (Board::PIECE_NO_TEAM[idx(piece)] == toInt(Piece::WP)) {
        bitboard::clearBit(pos.getPawns()[idx(color)], Board::SQ64[idx(square)]);
        bitboard::clearBit(pos.getPawns()[idx(toInt(Color::BOTH))], Board::SQ64[idx(square)]);
    }
    for (int i = 0; i < pos.getPieceNum(piece); i++) {
        if (pos.getPieceList(piece)[idx(i)] == square) {
            targetPNum = i;
            break;
        }
    }
    // Piece must be found in the piece list - if not, board state is corrupted
    if (targetPNum == -1) {
        throw std::runtime_error("Piece not found in piece list - corrupted board state");
    }
    pos.decrementPieceNum(piece);
    pos.getPieceList(piece)[idx(targetPNum)] = pos.getPieceList(piece)[idx(pos.getPieceNum(piece))];
}

inline void board::makemove::addPiece(const int piece, const int square, Board& pos)
{
    int color = Board::PIECE_COLOR[idx(piece)];
    pos.addMaterial(color, Board::PIECE_VAL[idx(piece)]);
    pos.setSquare(square, piece);
    pos.hashPiece(piece, square);
    if (Board::PIECE_NO_TEAM[idx(piece)] == toInt(Piece::WP)) {
        bitboard::setBit(pos.getPawns()[idx(color)], Board::SQ64[idx(square)]);
        bitboard::setBit(pos.getPawns()[idx(toInt(Color::BOTH))], Board::SQ64[idx(square)]);
    }
    // Check bounds before adding to piece list
    if (pos.getPieceNum(piece) >= PIECE_LIST_CAPACITY) {
        throw std::runtime_error("Piece list overflow - maximum number of pieces exceeded");
    }
    pos.getPieceList(piece)[idx(pos.getPieceNum(piece))] = square;
    pos.incrementPieceNum(piece);
}

inline void board::makemove::movePiece(const int from, const int to, Board& pos)
{
    int piece = pos.getSquare(from);
    int color = Board::PIECE_COLOR[idx(piece)];
    pos.hashPiece(piece, to);
    pos.hashPiece(piece, from);
    pos.setSquare(from, toInt(Piece::EMPTY));
    pos.setSquare(to, piece);
    if (Board::PIECE_NO_TEAM[idx(piece)] == toInt(Piece::WP)) {
        bitboard::clearBit(pos.getPawns()[idx(color)], Board::SQ64[idx(from)]);
        bitboard::clearBit(pos.getPawns()[idx(toInt(Color::BOTH))], Board::SQ64[idx(from)]);
        bitboard::setBit(pos.getPawns()[idx(color)], Board::SQ64[idx(to)]);
        bitboard::setBit(pos.getPawns()[idx(toInt(Color::BOTH))], Board::SQ64[idx(to)]);
    }
    for (int i = 0; i < pos.getPieceNum(piece); i++) {
        if (pos.getPieceList(piece)[idx(i)] == from) {
            pos.getPieceList(piece)[idx(i)] = to;
            break;
        }
    }
}

bool board::makemove::move(Move& move, Board& pos)
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
    if (move.getValue() & Move::MFLAGEP) {
        if (side == toInt(Color::WHITE)) {
            clearPiece(to - MAILBOX_RANK_STRIDE, pos);
        }
        else {
            clearPiece(to + MAILBOX_RANK_STRIDE, pos);
        }
    }
    else if (move.getValue() & Move::MFLAGCA) {
        if (to == toInt(Square::C1)) {
            movePiece(toInt(Square::A1), toInt(Square::D1), pos);
        }
        else if (to == toInt(Square::C8)) {
            movePiece(toInt(Square::A8), toInt(Square::D8), pos);
        }
        else if (to == toInt(Square::G1)) {
            movePiece(toInt(Square::H1), toInt(Square::F1), pos);
        }
        else if (to == toInt(Square::G8)) {
            movePiece(toInt(Square::H8), toInt(Square::F8), pos);
        }
    }
    if (pos.getEnPas() != toInt(Square::NO_SQ)) {
        pos.hashEnPas();
    }
    pos.hashCastle();
    pos.addHistory(current);
    pos.updateCastlePerm(to, from);
    pos.hashCastle();
    pos.clearEnPas();
    pos.incrementFiftyMove();
    if (captured != toInt(Piece::EMPTY)) {
        clearPiece(to, pos);
        pos.resetFiftyMove();
    }
    pos.incrementPly();
    if (Board::PIECE_NO_TEAM[idx(piece)] == toInt(Piece::WP)) {
        pos.resetFiftyMove();
        if (move.getValue() & Move::MFLAGPS) {
            if (side == toInt(Color::WHITE)) {
                pos.setEnPas(from + MAILBOX_RANK_STRIDE);
            }
            else {
                pos.setEnPas(from - MAILBOX_RANK_STRIDE);
            }
            pos.hashEnPas();
        }
    }
    movePiece(from, to, pos);
    if (promoted != toInt(Piece::EMPTY)) {
        clearPiece(to, pos);
        addPiece(promoted, to, pos);
    }
    pos.updateSide();
    pos.hashSide();
    if (pos.getSide() == toInt(Color::BLACK) &&
        pos.sqAttacked(pos.getPieceList(toInt(Piece::WK))[idx(0)], toInt(Color::BLACK))) {
        undo(pos);
        return false;
    }
    if (pos.getSide() == toInt(Color::WHITE) &&
        pos.sqAttacked(pos.getPieceList(toInt(Piece::BK))[idx(0)], toInt(Color::WHITE))) {
        undo(pos);
        return false;
    }
    return true;
}

void board::makemove::undo(Board& pos)
{
    pos.decrementPly();
    Undo undo = pos.popHistory();
    MoveValue move = undo.getMoveValue();
    int from = Move::FROMSQ(move);
    int to = Move::TOSQ(move);
    int captured = Move::CAPTURED(move);
    int promoted = Move::PROMOTED(move);
    pos.setFiftyMove(undo.getFiftyMove());
    if (pos.getEnPas() != toInt(Square::NO_SQ)) {
        pos.hashEnPas();
    }
    pos.setEnPas(undo.getEnPas());
    if (pos.getEnPas() != toInt(Square::NO_SQ)) {
        pos.hashEnPas();
    }
    pos.hashCastle();
    pos.setCastlePerm(undo.getCastlePerm());
    pos.hashCastle();
    pos.updateSide();
    pos.hashSide();
    if (Move::MFLAGEP & move) {
        if (pos.getSide() == toInt(Color::WHITE)) {
            addPiece(toInt(Piece::BP), to - MAILBOX_RANK_STRIDE, pos);
        }
        else {
            addPiece(toInt(Piece::WP), to + MAILBOX_RANK_STRIDE, pos);
        }
    }
    else if (Move::MFLAGCA & move) {
        if (to == toInt(Square::C1)) {
            movePiece(toInt(Square::D1), toInt(Square::A1), pos);
        }
        else if (to == toInt(Square::C8)) {
            movePiece(toInt(Square::D8), toInt(Square::A8), pos);
        }
        else if (to == toInt(Square::G1)) {
            movePiece(toInt(Square::F1), toInt(Square::H1), pos);
        }
        else if (to == toInt(Square::G8)) {
            movePiece(toInt(Square::F8), toInt(Square::H8), pos);
        }
    }
    movePiece(to, from, pos);
    if (captured != toInt(Piece::EMPTY) && !(Move::MFLAGEP & move)) {
        addPiece(captured, to, pos);
    }
    if (promoted != toInt(Piece::EMPTY)) {
        clearPiece(from, pos);
        addPiece(Board::PIECE_COLOR[idx(promoted)] == toInt(Color::WHITE) ? toInt(Piece::WP) : toInt(Piece::BP), from,
                 pos);
    }
}
