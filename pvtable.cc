#include "pvtable.h"

void PVTable::addMove(Board &pos, Move &move)
{
    uint64_t hash = pos.getHashKey();
    map[hash] = move;
}

Move PVTable::getMove(Board &pos) const
{
    uint64_t hash = pos.getHashKey();
    std::unordered_map<uint64_t, Move>::const_iterator element = map.find(hash);
    Move move;
    if(element != map.end()) {
        move = element->second;
    }
    return move;
}

std::vector<Move> PVTable::getPV(Board &pos)
{
    int originalPly = pos.getPly();
    std::vector<Move> pv;
    for(int i = 0; i < 64; i++) {
        Move move = getMove(pos);
        if(move.getValue() != 0 && makeMove(move, pos)) {
            pv.push_back(move);
        }
        else {
            break;
        }
    }
    while(pos.getPly() > originalPly) {
        takeMove(pos);
    }
    return pv;
}
