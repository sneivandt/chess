#include "utils.h"

std::string utils::getTimestamp()
{
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%X", &tstruct);
    return buf;
}

long utils::getTime()
{
    return std::chrono::system_clock::now().time_since_epoch()/std::chrono::milliseconds(1);
}
