#include "search/search.h"

#include "board/board.h"
#include "board/evaluate.h"
#include "board/makemove.h"
#include "board/move.h"
#include "pvtable.h"
#include "search/movegen.h"
#include "search/movelist.h"
#include "search/searchinfo.h"
#include "utils/utils.h"

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

void search::checkup(SearchInfo& info)
{
    if (info.getStopTime() > -1 && utils::getTime() > info.getStopTime()) {
        info.setStopped(true);
    }
    if (utils::inputWaiting()) {
        info.setStopped(true);
        std::string input;
        std::getline(std::cin, input);
        if (input == "quit") {
            info.setQuit(true);
        }
    }
}

bool search::isRepetition(board::Board& pos)
{
    size_t historySize = pos.getHistory().size();
    size_t fiftyMove = static_cast<size_t>(pos.getFiftyMove());
    
    // No repetition possible if history is empty or fiftyMove is 0
    if (historySize == 0 || fiftyMove == 0) {
        return false;
    }
    
    // Prevent unsigned underflow
    if (fiftyMove > historySize) {
        fiftyMove = historySize;
    }
    
    // Check only positions within the fifty move rule window
    for (size_t i = historySize - fiftyMove; i < historySize; i++) {
        if (pos.getHashKey() == pos.getHistory()[i].getHashKey()) {
            return true;
        }
    }
    return false;
}

void search::reset(board::Board& pos, SearchInfo& info)
{
    pos.clearSearchHistory();
    pos.clearSearchKillers();
    pos.resetPly();
    info.reset();
}

int search::negamax(int alpha, int beta, int depth, board::Board& pos, SearchInfo& info, PVTable& pvtable)
{
    if ((info.getNodes() & 2047) == 0) {
        checkup(info);
    }
    info.incrementNodes();
    if ((isRepetition(pos) || pos.getFiftyMove() >= 100) && pos.getPly() > 0) {
        return 0;
    }
    int kingPiece = pos.getSide() == board::WHITE ? board::WK : board::BK;
    if (pos.getPieceNum(kingPiece) == 0) {
        // No king on the board - this should never happen in a valid position
        return 0;
    }
    bool inCheck = pos.sqAttacked(pos.getPieceList(kingPiece)[0], pos.getSide() ^ 1);
    if (inCheck) {
        depth++;
    }
    if (depth == 0) {
        int score = board::evaluate::score(pos);
        if (score >= beta) {
            return beta;
        }
        if (score > alpha) {
            alpha = score;
        }
    }
    std::vector<board::Move> moves = movegen::generateAll(pos, depth == 0).getMoves();
    board::Move pvMove = pvtable.getMove(pos);
    if (pvMove.getValue() != 0) {
        for (board::Move move : moves) {
            if (pvMove.getValue() == move.getValue()) {
                move.addScore(2000000);
                break;
            }
        }
    }
    board::Move bestMove(0, 0); // Initialize to avoid using uninitialized value
    int legal = 0;
    int oldAlpha = alpha;
    int score = NEG_INFINITY;
    std::sort(moves.rbegin(), moves.rend());
    for (board::Move move : moves) {
        if (!board::makemove::move(move, pos)) {
            continue;
        }
        legal++;
        score = -negamax(-beta, -alpha, depth > 0 ? depth - 1 : 0, pos, info, pvtable);
        board::makemove::undo(pos);
        if (info.getStopped()) {
            return 0;
        }
        if (score > alpha) {
            if (score >= beta) {
                if (depth > 0) {
                    if (!(move.getValue() & board::Move::MFLAGCAP)) {
                        pos.addSearchKiller(move.getValue());
                    }
                }
                return beta;
            }
            alpha = score;
            bestMove = move;
            if (depth > 0) {
                if (!(move.getValue() & board::Move::MFLAGCAP)) {
                    pos.incrementSearchHistory(board::Move::TOSQ(move.getValue()), board::Move::FROMSQ(move.getValue()),
                                               depth);
                }
            }
        }
    }
    if (depth > 0 && legal == 0) {
        if (inCheck) {
            return -POS_INFINITY + pos.getPly();
        }
        return 0;
    }
    if (alpha != oldAlpha) {
        pvtable.addMove(pos, bestMove);
    }
    return alpha;
}

void search::go(board::Board& pos, SearchInfo& info)
{
    int score = NEG_INFINITY;
    PVTable pvtable;
    std::vector<board::Move> pv;
    reset(pos, info);
    for (int depth = 1; info.getDepth() == -1 || depth <= info.getDepth(); depth++) {
        score = negamax(NEG_INFINITY, POS_INFINITY, depth, pos, info, pvtable);
        if (info.getStopped()) {
            break;
        }
        pv = pvtable.getPV(pos);
        std::cout << "info score cp " << score;
        std::cout << " depth " << depth;
        std::cout << " nodes " << info.getNodes();
        std::cout << " time " << (utils::getTime() - info.getStartTime());
        std::cout << " pv";
        for (board::Move move : pv) {
            std::cout << " " << move.getString();
        }
        std::cout << std::endl;
    }
    if (!pv.empty()) {
        std::cout << "bestmove " << pv[0].getString() << std::endl;
    }
}
