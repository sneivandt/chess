#include "utils/utils.h"

std::string utils::getTimestamp()
{
    time_t now = time(nullptr);
    struct tm tstruct = tm();
#ifdef WIN32
    localtime_s(&tstruct, &now);
#else
    tstruct = *localtime(&now);
#endif
    char buf[80];
    strftime(static_cast<char*>(buf), sizeof(buf), "%X", &tstruct);
    return std::string(static_cast<char*>(buf));
}

int64_t utils::getTime()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
        .count();
}

bool utils::inputWaiting()
{
#ifdef WIN32
    static int init = 0, pipe;
    static HANDLE inh;
    DWORD dw;
    if (!init) {
        init = 1;
        inh = GetStdHandle(STD_INPUT_HANDLE);
        pipe = !GetConsoleMode(inh, &dw);
        if (!pipe) {
            SetConsoleMode(inh, dw & ~(ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT));
            FlushConsoleInputBuffer(inh);
        }
    }
    if (pipe) {
        if (!PeekNamedPipe(inh, NULL, 0, NULL, &dw, NULL)) {
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
    struct timeval tv
    {
        0, 1
    };
    FD_ZERO(&fds);
    FD_SET(fileno(stdin), &fds);
    return select(sizeof(fds) * 8, &fds, nullptr, nullptr, &tv);
#endif
}
