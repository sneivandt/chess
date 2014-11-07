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

void clearForSearch(Board& pos, SearchInfo& info)
{
    pos.clearSearchHistory();
    pos.clearSearchKillers();
    pos.resetPly();
    info.reset();
}

int alphaBeta(int alpha, int beta, const int depth, Board& pos, SearchInfo& info, PVTable& pvtable)
{
    if(depth == 0) {
        info.incrementNodes();
        return evaluatePosition(pos);
    }
    info.incrementNodes();
    if(isRepetition(pos) || pos.getFiftyMove() >= 100) {
        return 0;
    }
    bool foundMove = false;
    int oldAlpha = alpha;
    int score = NEG_INFINITY;
    Move bestMove;
    std::vector<Move> moves = generateAllMoves(pos).getMoves();
    for(unsigned int i = 0; i < moves.size(); i++) {
        if(!makeMove(moves[i], pos)) {
            continue;
        }
        foundMove |= true;
        score = -alphaBeta(-beta, -alpha, depth - 1, pos, info, pvtable);
        takeMove(pos);
        if(score > alpha) {
            if(score >= beta) {
                return beta;
            }
            alpha = score;
            bestMove = moves[i];
        }
    }
    if(!foundMove) {
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

void searchPosition(Board& pos, SearchInfo& info)
{
    int bestScore = NEG_INFINITY;
    PVTable pvtable;
    std::vector<Move> pv;
    clearForSearch(pos, info);
    for(int currentDepth = 1; currentDepth <= info.getDepth(); currentDepth++) {
        bestScore = alphaBeta(NEG_INFINITY, POS_INFINITY, currentDepth, pos, info, pvtable);
        pv = pvtable.getPV(pos);
        std::cout.width(3);
        std::cout << currentDepth << " ";
        std::cout.width(5);
        std::cout << bestScore << " ";
        for(unsigned int i = 0; i < pv.size(); i++) {
            std::cout.width(5);
            std::cout << pv[i].getString() << " ";
        }
        std::cout << std::endl;
    }
}
