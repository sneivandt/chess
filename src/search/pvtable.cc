#include "search/pvtable.h"

#include "board/board.h"
#include "board/makemove.h"

#include <cstdint>

void search::PVTable::addMove(const board::Board& pos, const board::Move& move)
{
    uint64_t hash = pos.getHashKey();
    map[hash] = move;
}

std::optional<board::Move> search::PVTable::getMove(const board::Board& pos) const
{
    uint64_t hash = pos.getHashKey();
    auto element = map.find(hash);
    if (element != map.end()) {
        return element->second;
    }
    return std::nullopt;
}

std::vector<board::Move> search::PVTable::getPV(board::Board& pos)
{
    int originalPly = pos.getPly();
    std::vector<board::Move> pv;
    for (int i = 0; i < board::BOARD64_SQUARE_NUM; i++) {
        auto move = getMove(pos);
        if (move && board::makemove::move(*move, pos)) {
            pv.push_back(*move);
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
