#include "search.h"

void search::checkup(SearchInfo &info)
{
    if(info.getStopTime() > -1 && utils::getTime() > info.getStopTime()) {
        info.setStopped(true);
    }
    // else if(std::cin.peek() != -1) {
    //     info.setStopped(true);
    //     std::string input;
    //     std::getline(std::cin, input);
    //     if(input == "quit") {
    //         info.setQuit(true);
    //     }
    // }
}

bool search::isRepetition(Board& pos)
{
    for(unsigned int i = pos.getHistory().size() - pos.getFiftyMove(); i < pos.getHistory().size(); i++) {
        if(pos.getHashKey() == pos.getHistory()[i].getHashKey()) {
            return true;
        }
    }
    return false;
}

void search::reset(Board &pos, SearchInfo &info)
{
    pos.clearSearchHistory();
    pos.clearSearchKillers();
    pos.resetPly();
    info.reset();
}

int search::negamax(int alpha, int beta, const int depth, Board &pos, SearchInfo &info, PVTable &pvtable)
{
    if((info.getNodes() & 2047) == 0) {
        checkup(info);
    }
    info.incrementNodes();
    if((isRepetition(pos) || pos.getFiftyMove() >= 100) && pos.getPly() > 0) {
        return 0;
    }
    if(depth == 0) {
        int score = evaluate::evaluatePosition(pos);
        if(score >= beta) {
            return beta;
        }
        if(score > alpha) {
            alpha = score;
        }
    }
    std::vector<Move> moves = generateAllMoves(pos, depth == 0).getMoves();
    Move pvMove = pvtable.getMove(pos);
    if(pvMove.getValue() != 0) {
        for(unsigned int i = 0; i < moves.size(); i++) {
            if(pvMove.getValue() == moves[i].getValue()) {
                moves[i].addScore(2000000);
                break;
            }
        }
    }
    Move bestMove;
    int legal = 0;
    int oldAlpha = alpha;
    int score = NEG_INFINITY;
    std::sort(moves.rbegin(), moves.rend());
    for(unsigned int i = 0; i < moves.size(); i++) {
        if(!makeMove(moves[i], pos)) {
            continue;
        }
        legal++;
        score = -negamax(-beta, -alpha, depth > 0 ? depth - 1 : 0, pos, info, pvtable);
        takeMove(pos);
        if(info.getStopped()) {
            return 0;
        }
        if(score > alpha) {
            if(score >= beta) {
                if(legal == 1) {
                    info.incrementFailHighFirst();
                }
                info.incrementFailHigh();
                if(depth > 0) {
                    if(!(moves[i].getValue() & MFLAGCAP)) {
                        pos.addSearchKiller(moves[i].getValue());
                    }
                }
                return beta;
            }
            alpha = score;
            bestMove = moves[i];
            if(depth > 0) {
                if(!(moves[i].getValue() & MFLAGCAP)) {
                    pos.incrementSearchHistory(moves[i].getValue(), depth);
                }
            }
        }
    }
    if(depth > 0 && legal == 0) {
        if(pos.sqAttacked(pos.getPieceList(pos.getSide() == WHITE ? WK : BK)[0], pos.getSide() ^ 1)) {
            return -MATE + pos.getPly();
        }
        return 0;
    }
    if(alpha != oldAlpha) {
        pvtable.addMove(pos, bestMove);
    }
    return alpha;
}

void search::searchPosition(Board &pos, SearchInfo &info)
{
    int score = NEG_INFINITY;
    PVTable pvtable;
    std::vector<Move> pv;
    reset(pos, info);
    for(int depth = 1; info.getDepth() == -1 || depth <= info.getDepth(); depth++) {
        score = negamax(NEG_INFINITY, POS_INFINITY, depth, pos, info, pvtable);
        if(info.getStopped()) {
            break;
        }
        pv = pvtable.getPV(pos);
        std::cout << "info score cp " << score << " depth " << depth << " nodes " << info.getNodes() << " time " << (utils::getTime() - info.getStartTime()) << " pv";
        for(unsigned int i = 0; i < pv.size(); i++) {
            std::cout << " " << pv[i].getString();
        }
        std::cout << std::endl;
    }
    if(pv.size() > 0) {
        std::cout << "bestmove " << pv[0].getString() << std::endl;
    }
}
