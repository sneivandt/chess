#include "searchinfo.h"

void SearchInfo::reset()
{
    startTime = getTime();
    stopped = false;
    nodes = 0;
}
