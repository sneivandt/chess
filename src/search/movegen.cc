#include "search/movegen.h"

#include "board/move.h"

int search::movegen::MVVLVA_SCORES[13][13];

void search::movegen::INIT()
{
    for (int attacker = board::WP; attacker <= board::BK; attacker++) {
        for (int victim = board::WP; victim < board::BK; victim++) {
            MVVLVA_SCORES[victim][attacker] = VICTUM_SCORES[victim] + 6 - (VICTUM_SCORES[attacker] / 100);
        }
    }
}

inline void search::movegen::addQuietMove(const int move, MoveList& list, board::Board& pos)
{
    int score = 0;
    if (pos.getSearchKiller(0) == move) {
        score = 900000;
    }
    else if (pos.getSearchKiller(1) == move) {
        score = 800000;
    }
    else {
        score = pos.getSearchHistory(pos.getSquare(board::Move::FROMSQ(move)), board::Move::TOSQ(move));
    }
    list.addMove(board::Move(move, score));
}

inline void search::movegen::addCaptureMove(const int move, MoveList& list, board::Board& pos)
{
    list.addMove(board::Move(
        move, MVVLVA_SCORES[board::Move::CAPTURED(move)][pos.getSquare(board::Move::FROMSQ(move))] + 1000000));
}

inline void search::movegen::addEnPasMove(const int move, MoveList& list)
{
    list.addMove(board::Move(move, MVVLVA_SCORES[board::WP][board::WP] + 1000000));
}

inline void search::movegen::addPawnMove(const int from, const int to, const int cap, const int side, MoveList& list,
                                         board::Board& pos)
{
    if (board::Board::RANKS[board::Board::SQ64[from]] == PAWN_RANK[side]) {
        for (int i = 0; i < 4; i++) {
            addCaptureMove(board::Move::MOVE(from, to, cap, PROMOTION_PIECES[side][i], 0), list, pos);
        }
    }
    else {
        addCaptureMove(board::Move::MOVE(from, to, cap, board::EMPTY, 0), list, pos);
    }
}

search::MoveList search::movegen::generateAll(board::Board& pos, const bool onlyCaps)
{
    MoveList moves;
    int square;
    int piece;
    int offset;
    int direction;
    int targetSquare;
    pos.getSide() == board::WHITE ? piece = board::WP : piece = board::BP;
    pos.getSide() == board::WHITE ? offset = 1 : offset = -1;
    for (int pceNum = 0; pceNum < pos.getPieceNum(piece); pceNum++) {
        square = pos.getPieceList(piece)[pceNum];
        if (!onlyCaps) {
            if (pos.getSquare(square + offset * 10) == board::EMPTY) {
                addPawnMove(square, square + offset * 10, board::EMPTY, pos.getSide(), moves, pos);
                if (board::Board::RANKS[board::Board::SQ64[square]] == PAWN_RANK[(pos.getSide() ^ 1)] &&
                    pos.getSquare(square + offset * 20) == board::EMPTY) {
                    addQuietMove(board::Move::MOVE(square, square + offset * 20, board::EMPTY, board::EMPTY,
                                                   board::Move::MFLAGPS),
                                 moves, pos);
                }
            }
        }
        if (board::Board::SQ64[square + offset * 9] != board::NO_SQ &&
            board::Board::PIECE_COLOR[pos.getSquare(square + offset * 9)] == (pos.getSide() ^ 1)) {
            addPawnMove(square, square + offset * 9, pos.getSquare(square + offset * 9), pos.getSide(), moves, pos);
        }
        if (board::Board::SQ64[square + offset * 11] != board::NO_SQ &&
            board::Board::PIECE_COLOR[pos.getSquare(square + offset * 11)] == (pos.getSide() ^ 1)) {
            addPawnMove(square, square + offset * 11, pos.getSquare(square + offset * 11), pos.getSide(), moves, pos);
        }
        if (pos.getEnPas() != board::NO_SQ) {
            if (square + offset * 9 == pos.getEnPas()) {
                addEnPasMove(
                    board::Move::MOVE(square, square + offset * 9, board::EMPTY, board::EMPTY, board::Move::MFLAGEP),
                    moves);
            }
            if (square + offset * 11 == pos.getEnPas()) {
                addEnPasMove(
                    board::Move::MOVE(square, square + offset * 11, board::EMPTY, board::EMPTY, board::Move::MFLAGEP),
                    moves);
            }
        }
    }
    pos.getSide() == board::WHITE ? offset = 0 : offset = 3;
    for (int index = 0; index < 3; index++) {
        piece = SLIDERS[index + offset];
        for (int pceNum = 0; pceNum < pos.getPieceNum(piece); pceNum++) {
            square = pos.getPieceList(piece)[pceNum];
            for (int i = 0; i < 8 && board::Board::MOVE_DIR[piece][i] != 0; i++) {
                direction = board::Board::MOVE_DIR[piece][i];
                targetSquare = square + direction;
                while (pos.getSquare(targetSquare) != board::NO_SQ) {
                    if (pos.getSquare(targetSquare) != board::EMPTY) {
                        if (board::Board::PIECE_COLOR[pos.getSquare(targetSquare)] == (pos.getSide() ^ 1)) {
                            addCaptureMove(
                                board::Move::MOVE(square, targetSquare, pos.getSquare(targetSquare), board::EMPTY, 0),
                                moves, pos);
                        }
                        break;
                    }
                    if (!onlyCaps) {
                        addQuietMove(board::Move::MOVE(square, targetSquare, board::EMPTY, board::EMPTY, 0), moves,
                                     pos);
                    }
                    targetSquare += direction;
                }
            }
        }
    }
    pos.getSide() == board::WHITE ? offset = 0 : offset = 2;
    for (int index = 0; index < 2; index++) {
        piece = NON_SLIDERS[index + offset];
        for (int pceNum = 0; pceNum < pos.getPieceNum(piece); pceNum++) {
            square = pos.getPieceList(piece)[pceNum];
            for (int i = 0; i < 8 && board::Board::MOVE_DIR[piece][i] != 0; i++) {
                direction = board::Board::MOVE_DIR[piece][i];
                targetSquare = square + direction;
                if (pos.getSquare(targetSquare) == board::NO_SQ) {
                    continue;
                }
                if (pos.getSquare(targetSquare) != board::EMPTY) {
                    if (board::Board::PIECE_COLOR[pos.getSquare(targetSquare)] == (pos.getSide() ^ 1)) {
                        addCaptureMove(
                            board::Move::MOVE(square, targetSquare, pos.getSquare(targetSquare), board::EMPTY, 0),
                            moves, pos);
                    }
                    continue;
                }
                if (!onlyCaps) {
                    addQuietMove(board::Move::MOVE(square, targetSquare, board::EMPTY, board::EMPTY, 0), moves, pos);
                }
            }
        }
    }
    if (!onlyCaps) {
        if (pos.getSide() == board::WHITE) {
            if (pos.getCastlePerm() & board::WKCA) {
                if (pos.getSquare(board::F1) == board::EMPTY && pos.getSquare(board::G1) == board::EMPTY) {
                    if (!pos.sqAttacked(board::E1, board::BLACK) && !pos.sqAttacked(board::F1, board::BLACK)) {
                        addQuietMove(
                            board::Move::MOVE(board::E1, board::G1, board::EMPTY, board::EMPTY, board::Move::MFLAGCA),
                            moves, pos);
                    }
                }
            }
            if (pos.getCastlePerm() & board::WQCA) {
                if (pos.getSquare(board::D1) == board::EMPTY && pos.getSquare(board::C1) == board::EMPTY &&
                    pos.getSquare(board::B1) == board::EMPTY) {
                    if (!pos.sqAttacked(board::E1, board::BLACK) && !pos.sqAttacked(board::D1, board::BLACK)) {
                        addQuietMove(
                            board::Move::MOVE(board::E1, board::C1, board::EMPTY, board::EMPTY, board::Move::MFLAGCA),
                            moves, pos);
                    }
                }
            }
        }
        else {
            if (pos.getCastlePerm() & board::BKCA) {
                if (pos.getSquare(board::F8) == board::EMPTY && pos.getSquare(board::G8) == board::EMPTY) {
                    if (!pos.sqAttacked(board::E8, board::WHITE) && !pos.sqAttacked(board::F8, board::WHITE)) {
                        addQuietMove(
                            board::Move::MOVE(board::E8, board::G8, board::EMPTY, board::EMPTY, board::Move::MFLAGCA),
                            moves, pos);
                    }
                }
            }
            if (pos.getCastlePerm() & board::BQCA) {
                if (pos.getSquare(board::D8) == board::EMPTY && pos.getSquare(board::C8) == board::EMPTY &&
                    pos.getSquare(board::B8) == board::EMPTY) {
                    if (!pos.sqAttacked(board::E8, board::WHITE) && !pos.sqAttacked(board::D8, board::WHITE)) {
                        addQuietMove(
                            board::Move::MOVE(board::E8, board::C8, board::EMPTY, board::EMPTY, board::Move::MFLAGCA),
                            moves, pos);
                    }
                }
            }
        }
    }
    return moves;
}
