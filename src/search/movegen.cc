#include "search/movegen.h"

#include "board/move.h"
#include "search/searchstate.h"

using board::idx;

std::array<std::array<int, board::PIECE_TYPE_NUM>, board::PIECE_TYPE_NUM> search::movegen::MVVLVA_SCORES;

void search::movegen::INIT()
{
    for (int attacker = board::toInt(board::Piece::WP); attacker <= board::toInt(board::Piece::BK); attacker++) {
        for (int victim = board::toInt(board::Piece::WP); victim <= board::toInt(board::Piece::BK); victim++) {
            MVVLVA_SCORES[idx(victim)][idx(attacker)] =
                VICTUM_SCORES[idx(victim)] + 6 - (VICTUM_SCORES[idx(attacker)] / 100);
        }
    }
}

inline void search::movegen::addQuietMove(const int move, MoveList& list, board::Board& pos, const SearchState* ss)
{
    int score = 0;
    if (ss) {
        if (ss->getKiller(0, pos.getPly()) == move) {
            score = PRIMARY_KILLER_SCORE;
        }
        else if (ss->getKiller(1, pos.getPly()) == move) {
            score = SECONDARY_KILLER_SCORE;
        }
        else {
            score = ss->getHistory(pos.getSquare(board::Move::FROMSQ(move)), board::Move::TOSQ(move));
        }
    }
    list.addMove(board::Move(move, score));
}

inline void search::movegen::addCaptureMove(const int move, MoveList& list, board::Board& pos)
{
    list.addMove(board::Move(
        move, MVVLVA_SCORES[idx(board::Move::CAPTURED(move))][idx(pos.getSquare(board::Move::FROMSQ(move)))] +
                  CAPTURE_SCORE_OFFSET));
}

inline void search::movegen::addEnPasMove(const int move, MoveList& list)
{
    list.addMove(
        board::Move(move, MVVLVA_SCORES[idx(board::toInt(board::Piece::WP))][idx(board::toInt(board::Piece::WP))] +
                              CAPTURE_SCORE_OFFSET));
}

inline void search::movegen::addPawnMove(const int from, const int to, const int cap, const int side, MoveList& list,
                                         board::Board& pos, const SearchState* ss)
{
    if (board::Board::RANKS[idx(board::Board::SQ64[idx(from)])] == PAWN_RANK[idx(side)]) {
        for (int i = 0; i < 4; i++) {
            addCaptureMove(board::Move::MOVE(from, to, cap, PROMOTION_PIECES[idx(side)][idx(i)], 0), list, pos);
        }
    }
    else {
        if (cap == board::toInt(board::Piece::EMPTY)) {
            addQuietMove(board::Move::MOVE(from, to, cap, board::toInt(board::Piece::EMPTY), 0), list, pos, ss);
        }
        else {
            addCaptureMove(board::Move::MOVE(from, to, cap, board::toInt(board::Piece::EMPTY), 0), list, pos);
        }
    }
}

void search::movegen::generatePawnMoves(board::Board& pos, MoveList& moves, const bool onlyCaps, const SearchState* ss)
{
    int piece;
    int offset;
    int square;

    pos.getSide() == board::toInt(board::Color::WHITE) ? piece = board::toInt(board::Piece::WP)
                                                       : piece = board::toInt(board::Piece::BP);
    pos.getSide() == board::toInt(board::Color::WHITE) ? offset = 1 : offset = -1;

    for (int pceNum = 0; pceNum < pos.getPieceNum(piece); pceNum++) {
        square = pos.getPieceList(piece)[idx(pceNum)];
        if (!onlyCaps) {
            if (pos.getSquare(square + offset * board::MAILBOX_RANK_STRIDE) == board::toInt(board::Piece::EMPTY)) {
                addPawnMove(square, square + offset * board::MAILBOX_RANK_STRIDE, board::toInt(board::Piece::EMPTY),
                            pos.getSide(), moves, pos, ss);
                if (board::Board::RANKS[idx(board::Board::SQ64[idx(square)])] == PAWN_START_RANK[idx(pos.getSide())] &&
                    pos.getSquare(square + offset * board::MAILBOX_RANK_STRIDE * 2) ==
                        board::toInt(board::Piece::EMPTY)) {
                    addQuietMove(board::Move::MOVE(square, square + offset * board::MAILBOX_RANK_STRIDE * 2,
                                                   board::toInt(board::Piece::EMPTY), board::toInt(board::Piece::EMPTY),
                                                   board::Move::MFLAGPS),
                                 moves, pos, ss);
                }
            }
        }
        if (board::Board::SQ64[idx(square + offset * 9)] != board::OFFBOARD &&
            board::Board::PIECE_COLOR[idx(pos.getSquare(square + offset * 9))] == (pos.getSide() ^ 1)) {
            addPawnMove(square, square + offset * 9, pos.getSquare(square + offset * 9), pos.getSide(), moves, pos, ss);
        }
        if (board::Board::SQ64[idx(square + offset * 11)] != board::OFFBOARD &&
            board::Board::PIECE_COLOR[idx(pos.getSquare(square + offset * 11))] == (pos.getSide() ^ 1)) {
            addPawnMove(square, square + offset * 11, pos.getSquare(square + offset * 11), pos.getSide(), moves, pos,
                        ss);
        }
        if (pos.getEnPas() != board::toInt(board::Square::NO_SQ)) {
            if (square + offset * 9 == pos.getEnPas()) {
                addEnPasMove(board::Move::MOVE(square, square + offset * 9, board::toInt(board::Piece::EMPTY),
                                               board::toInt(board::Piece::EMPTY), board::Move::MFLAGEP),
                             moves);
            }
            if (square + offset * 11 == pos.getEnPas()) {
                addEnPasMove(board::Move::MOVE(square, square + offset * 11, board::toInt(board::Piece::EMPTY),
                                               board::toInt(board::Piece::EMPTY), board::Move::MFLAGEP),
                             moves);
            }
        }
    }
}

void search::movegen::generateSliderMoves(board::Board& pos, MoveList& moves, const bool onlyCaps,
                                          const SearchState* ss)
{
    int offset;
    int piece;
    int square;
    int direction;
    int targetSquare;

    pos.getSide() == board::toInt(board::Color::WHITE) ? offset = 0 : offset = 3;
    for (int index = 0; index < 3; index++) {
        piece = SLIDERS[idx(index + offset)];
        for (int pceNum = 0; pceNum < pos.getPieceNum(piece); pceNum++) {
            square = pos.getPieceList(piece)[idx(pceNum)];
            for (int i = 0; i < board::MOVE_DIRECTION_NUM && board::Board::MOVE_DIR[idx(piece)][idx(i)] != 0; i++) {
                direction = board::Board::MOVE_DIR[idx(piece)][idx(i)];
                targetSquare = square + direction;
                while (pos.getSquare(targetSquare) != board::OFFBOARD) {
                    if (pos.getSquare(targetSquare) != board::toInt(board::Piece::EMPTY)) {
                        if (board::Board::PIECE_COLOR[idx(pos.getSquare(targetSquare))] == (pos.getSide() ^ 1)) {
                            addCaptureMove(board::Move::MOVE(square, targetSquare, pos.getSquare(targetSquare),
                                                             board::toInt(board::Piece::EMPTY), 0),
                                           moves, pos);
                        }
                        break;
                    }
                    if (!onlyCaps) {
                        addQuietMove(board::Move::MOVE(square, targetSquare, board::toInt(board::Piece::EMPTY),
                                                       board::toInt(board::Piece::EMPTY), 0),
                                     moves, pos, ss);
                    }
                    targetSquare += direction;
                }
            }
        }
    }
}

void search::movegen::generateNonSliderMoves(board::Board& pos, MoveList& moves, const bool onlyCaps,
                                             const SearchState* ss)
{
    int offset;
    int piece;
    int square;
    int direction;
    int targetSquare;

    pos.getSide() == board::toInt(board::Color::WHITE) ? offset = 0 : offset = 2;
    for (int index = 0; index < 2; index++) {
        piece = NON_SLIDERS[idx(index + offset)];
        for (int pceNum = 0; pceNum < pos.getPieceNum(piece); pceNum++) {
            square = pos.getPieceList(piece)[idx(pceNum)];
            for (int i = 0; i < board::MOVE_DIRECTION_NUM && board::Board::MOVE_DIR[idx(piece)][idx(i)] != 0; i++) {
                direction = board::Board::MOVE_DIR[idx(piece)][idx(i)];
                targetSquare = square + direction;
                if (pos.getSquare(targetSquare) == board::OFFBOARD) {
                    continue;
                }
                if (pos.getSquare(targetSquare) != board::toInt(board::Piece::EMPTY)) {
                    if (board::Board::PIECE_COLOR[idx(pos.getSquare(targetSquare))] == (pos.getSide() ^ 1)) {
                        addCaptureMove(board::Move::MOVE(square, targetSquare, pos.getSquare(targetSquare),
                                                         board::toInt(board::Piece::EMPTY), 0),
                                       moves, pos);
                    }
                    continue;
                }
                if (!onlyCaps) {
                    addQuietMove(board::Move::MOVE(square, targetSquare, board::toInt(board::Piece::EMPTY),
                                                   board::toInt(board::Piece::EMPTY), 0),
                                 moves, pos, ss);
                }
            }
        }
    }
}

void search::movegen::generateCastlingMoves(board::Board& pos, MoveList& moves, const SearchState* ss)
{
    if (pos.getSide() == board::toInt(board::Color::WHITE)) {
        if (pos.getCastlePerm() & board::CastlePerm::WKCA) {
            if (pos.getSquare(board::toInt(board::Square::F1)) == board::toInt(board::Piece::EMPTY) &&
                pos.getSquare(board::toInt(board::Square::G1)) == board::toInt(board::Piece::EMPTY)) {
                if (!pos.sqAttacked(board::toInt(board::Square::E1), board::toInt(board::Color::BLACK)) &&
                    !pos.sqAttacked(board::toInt(board::Square::F1), board::toInt(board::Color::BLACK))) {
                    addQuietMove(board::Move::MOVE(board::toInt(board::Square::E1), board::toInt(board::Square::G1),
                                                   board::toInt(board::Piece::EMPTY), board::toInt(board::Piece::EMPTY),
                                                   board::Move::MFLAGCA),
                                 moves, pos, ss);
                }
            }
        }
        if (pos.getCastlePerm() & board::CastlePerm::WQCA) {
            if (pos.getSquare(board::toInt(board::Square::D1)) == board::toInt(board::Piece::EMPTY) &&
                pos.getSquare(board::toInt(board::Square::C1)) == board::toInt(board::Piece::EMPTY) &&
                pos.getSquare(board::toInt(board::Square::B1)) == board::toInt(board::Piece::EMPTY)) {
                if (!pos.sqAttacked(board::toInt(board::Square::E1), board::toInt(board::Color::BLACK)) &&
                    !pos.sqAttacked(board::toInt(board::Square::D1), board::toInt(board::Color::BLACK))) {
                    addQuietMove(board::Move::MOVE(board::toInt(board::Square::E1), board::toInt(board::Square::C1),
                                                   board::toInt(board::Piece::EMPTY), board::toInt(board::Piece::EMPTY),
                                                   board::Move::MFLAGCA),
                                 moves, pos, ss);
                }
            }
        }
    }
    else {
        if (pos.getCastlePerm() & board::CastlePerm::BKCA) {
            if (pos.getSquare(board::toInt(board::Square::F8)) == board::toInt(board::Piece::EMPTY) &&
                pos.getSquare(board::toInt(board::Square::G8)) == board::toInt(board::Piece::EMPTY)) {
                if (!pos.sqAttacked(board::toInt(board::Square::E8), board::toInt(board::Color::WHITE)) &&
                    !pos.sqAttacked(board::toInt(board::Square::F8), board::toInt(board::Color::WHITE))) {
                    addQuietMove(board::Move::MOVE(board::toInt(board::Square::E8), board::toInt(board::Square::G8),
                                                   board::toInt(board::Piece::EMPTY), board::toInt(board::Piece::EMPTY),
                                                   board::Move::MFLAGCA),
                                 moves, pos, ss);
                }
            }
        }
        if (pos.getCastlePerm() & board::CastlePerm::BQCA) {
            if (pos.getSquare(board::toInt(board::Square::D8)) == board::toInt(board::Piece::EMPTY) &&
                pos.getSquare(board::toInt(board::Square::C8)) == board::toInt(board::Piece::EMPTY) &&
                pos.getSquare(board::toInt(board::Square::B8)) == board::toInt(board::Piece::EMPTY)) {
                if (!pos.sqAttacked(board::toInt(board::Square::E8), board::toInt(board::Color::WHITE)) &&
                    !pos.sqAttacked(board::toInt(board::Square::D8), board::toInt(board::Color::WHITE))) {
                    addQuietMove(board::Move::MOVE(board::toInt(board::Square::E8), board::toInt(board::Square::C8),
                                                   board::toInt(board::Piece::EMPTY), board::toInt(board::Piece::EMPTY),
                                                   board::Move::MFLAGCA),
                                 moves, pos, ss);
                }
            }
        }
    }
}

search::MoveList search::movegen::generateAll(board::Board& pos, const bool onlyCaps, const SearchState* ss)
{
    MoveList moves;
    generatePawnMoves(pos, moves, onlyCaps, ss);
    generateSliderMoves(pos, moves, onlyCaps, ss);
    generateNonSliderMoves(pos, moves, onlyCaps, ss);
    if (!onlyCaps) {
        generateCastlingMoves(pos, moves, ss);
    }
    return moves;
}
