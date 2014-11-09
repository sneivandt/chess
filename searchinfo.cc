#include "searchinfo.h"

void SearchInfo::reset()
{
    startTime = getTime();
    stopped = false;
    nodes = 0;
    failHigh = 0;
    failHighFirst = 0;
}
