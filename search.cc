#include "search.h"

bool isRepetition(Board& pos)
{
    for(unsigned int i = pos.getHistory().size() - pos.getFiftyMove(); i < pos.getHistory().size(); i++) {
        if(pos.getHashKey() == pos.getHistory()[i].getHashKey()) {
            return true;
        }
    }
    return false;
}

void clearForSearch(Board &pos, SearchInfo &info)
{
    pos.clearSearchHistory();
    pos.clearSearchKillers();
    pos.resetPly();
    info.reset();
}

int quiescence(int alpha, int beta, Board &pos, SearchInfo &info, PVTable &pvtable)
{
    info.incrementNodes();
    if(isRepetition(pos) || pos.getFiftyMove() >= 100) {
        return 0;
    }
    int score = evaluatePosition(pos);
    if(score >= beta) {
        return beta;
    }
    if(score > alpha) {
        alpha = score;
    }
    std::vector<Move> moves = generateAllMoves(pos, true).getMoves();
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
    score = NEG_INFINITY;
    std::sort(moves.rbegin(), moves.rend());
    for(unsigned int i = 0; i < moves.size(); i++) {
        if(!makeMove(moves[i], pos)) {
            continue;
        }
        legal++;
        score = -quiescence(-beta, -alpha, pos, info, pvtable);
        takeMove(pos);
        if(score > alpha) {
            if(score >= beta) {
                if(legal == 1) {
                    info.incrementFailHighFirst();
                }
                info.incrementFailHigh();
                return beta;
            }
            alpha = score;
            bestMove = moves[i];
        }
    }
    if(alpha != oldAlpha) {
        pvtable.addMove(pos, bestMove);
    }
    return alpha;
}

int alphaBeta(int alpha, int beta, const int depth, Board &pos, SearchInfo &info, PVTable &pvtable)
{
    if(depth == 0) {
        return quiescence(alpha, beta, pos, info, pvtable);
    }
    info.incrementNodes();
    if(isRepetition(pos) || pos.getFiftyMove() >= 100) {
        return 0;
    }
    std::vector<Move> moves = generateAllMoves(pos, false).getMoves();
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
        score = -alphaBeta(-beta, -alpha, depth - 1, pos, info, pvtable);
        takeMove(pos);
        if(score > alpha) {
            if(score >= beta) {
                if(legal == 1) {
                    info.incrementFailHighFirst();
                }
                info.incrementFailHigh();
                if(!(moves[i].getValue() & MFLAGCAP)) {
                    pos.addSearchKiller(moves[i].getValue());
                }
                return beta;
            }
            alpha = score;
            bestMove = moves[i];
            if(!(moves[i].getValue() & MFLAGCAP)) {
                pos.incrementSearchHistory(moves[i].getValue(), depth);
            }
        }
    }
    if(legal == 0) {
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

void searchPosition(Board &pos, SearchInfo &info)
{
    int bestScore = NEG_INFINITY;
    PVTable pvtable;
    std::vector<Move> pv;
    clearForSearch(pos, info);
    for(int currentDepth = 1; currentDepth <= info.getDepth(); currentDepth++) {
        bestScore = alphaBeta(NEG_INFINITY, POS_INFINITY, currentDepth, pos, info, pvtable);
        if(pos.getSide() == BLACK) {
            bestScore = -bestScore;
        }
        pv = pvtable.getPV(pos);
        printSearch(info, currentDepth, bestScore, pv);
    }
}

void printSearch(SearchInfo &info, const int depth, const int score, std::vector<Move> pv)
{
    std::cout.width(2);
    std::cout << depth << " ";
    std::cout << std::fixed << std::setprecision(2) << info.getOrdering() << " ";
    std::cout.width(10);
    std::cout << info.getNodes() << " ";
    std::cout.width(6);
    std::cout << score << " ";
    for(unsigned int i = 0; i < pv.size(); i++) {
        std::cout.width(5);
        std::cout << std::left << pv[i].getString() << std::right << " ";
    }
    std::cout << std::endl;
}
