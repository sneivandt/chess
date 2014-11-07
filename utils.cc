#include "utils.h"

std::string getTimestamp()
{
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%X", &tstruct);
    return buf;
}

int getTime()
{
    return time(0) * 1000;
}
