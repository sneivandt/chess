#include "search/searchinfo.h"

void search::SearchInfo::reset()
{
    nodes = 0;
    stopped = false;
    quit = false;
}

void search::SearchInfo::calculateStopTime(const int time, const int increment)
{
    if (time == -1) {
        stopTime = -1;
    }
    else {
        stopTime = startTime + time + increment - 50;
    }
}
