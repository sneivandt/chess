#include "utils.h"

std::string utils::getTimestamp()
{
    time_t now = time(0);
    struct tm tstruct;
#ifdef WIN32
    localtime_s(&tstruct, &now);
#else
    localtime_r(&now, &tstruct);
#endif
    char buf[80];
    strftime(buf, sizeof(buf), "%X", &tstruct);
    return buf;
}

long utils::getTime()
{
#ifdef WIN32
    return GetTickCount();
#else
    return std::chrono::system_clock::now().time_since_epoch()/std::chrono::milliseconds(1);
#endif
}

bool utils::inputWaiting()
{
#ifdef WIN32
    static int init = 0, pipe;
    static HANDLE inh;
    DWORD dw;
    if(!init) {
        init = 1;
        inh = GetStdHandle(STD_INPUT_HANDLE);
        pipe = !GetConsoleMode(inh, &dw);
        if(!pipe) {
            SetConsoleMode(inh, dw & ~(ENABLE_MOUSE_INPUT|ENABLE_WINDOW_INPUT));
            FlushConsoleInputBuffer(inh);
        }
    }
    if(pipe) {
        if(!PeekNamedPipe(inh, NULL, 0, NULL, &dw, NULL)) {
            return 1;
        }
        return dw > 0;
    }
    else {
        GetNumberOfConsoleInputEvents(inh, &dw);
        return dw <= 1 ? false : dw > 0;
    }
#else
    fd_set fds;
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 1;
    FD_ZERO(&fds);
    FD_SET(fileno(stdin), &fds);
    return select(sizeof(fds) * 8, &fds, NULL, NULL, &tv);
#endif
}
