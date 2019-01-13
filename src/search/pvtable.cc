#include "pvtable.h"

#include "board/board.h"
#include "board/makemove.h"

#include <cstdint>
#include <utility>

void search::PVTable::addMove(board::Board& pos, board::Move& move)
{
    uint64_t hash = pos.getHashKey();
    map[hash] = move;
}

board::Move search::PVTable::getMove(board::Board& pos) const
{
    uint64_t hash = pos.getHashKey();
    auto element = map.find(hash);
    board::Move move;
    if (element != map.end()) {
        move = element->second;
    }
    return move;
}

std::vector<board::Move> search::PVTable::getPV(board::Board& pos)
{
    int originalPly = pos.getPly();
    std::vector<board::Move> pv;
    for (int i = 0; i < 64; i++) {
        board::Move move = getMove(pos);
        if (move.getValue() != 0 && board::makemove::move(move, pos)) {
            pv.push_back(move);
        }
        else {
            break;
        }
    }
    while (pos.getPly() > originalPly) {
        board::makemove::undo(pos);
    }
    return pv;
}
