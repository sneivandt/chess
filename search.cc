#include "search.h"

bool isRepetition(Board& pos)
{
    for(int i = pos.getHistoryPly() - pos.getFiftyMove(); i < pos.getHistoryPly(); i++) {
        if(pos.getHashKey() == pos.getHistory()[i].getHashKey()) {
            return true;
        }
    }
    return false;
}
