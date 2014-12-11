#include "searchinfo.h"

void SearchInfo::reset()
{
    nodes = 0;
    stopped = false;
    quit = false;
}

void SearchInfo::calculateStopTime(const int time, const int increment)
{
    if(time == -1) {
        stopTime = -1;
    }
    else {
        stopTime = startTime + time + increment - 50;
    }
}
