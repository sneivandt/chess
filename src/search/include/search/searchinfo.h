#ifndef SEARCHINFO_H
#define SEARCHINFO_H

#include "utils/utils.h"

#include <cstdint>

namespace search {

class SearchInfo
{
  private:
    // Depth of the search
    int depth = -1;

    // Search start time
    int64_t startTime = -1;

    // Search stop time
    int64_t stopTime = -1;

    // Nodes searched
    int64_t nodes = -1;

    // Interupt flag
    bool quit = false;

    // Stop flag
    bool stopped = false;

  public:
    // Constructors
    SearchInfo()
    {
        reset();
    };
    SearchInfo(const int depth) : depth(depth), stopTime(-1) {};

    // Reset the search info
    void reset();

    // Calclate the stop time
    void calculateStopTime(const int, const int);

    // Nodes
    int64_t getNodes() const;
    void incrementNodes();

    // Start time
    int64_t getStartTime() const;
    void setStartTime();

    // Stop time
    int64_t getStopTime() const;

    // Depth
    int getDepth() const;
    void setDepth(const int);

    // Stopped
    bool getStopped() const;
    void setStopped(const bool);

    // Quit
    bool getQuit() const;
    void setQuit(const bool);
};

inline int64_t SearchInfo::getNodes() const
{
    return nodes;
}

inline void SearchInfo::incrementNodes()
{
    nodes++;
}

inline int64_t SearchInfo::getStartTime() const
{
    return startTime;
}

inline void SearchInfo::setStartTime()
{
    startTime = utils::getTime();
}

inline int64_t SearchInfo::getStopTime() const
{
    return stopTime;
}

inline int SearchInfo::getDepth() const
{
    return depth;
}

inline void SearchInfo::setDepth(const int d)
{
    depth = d;
}

inline bool SearchInfo::getStopped() const
{
    return stopped;
}

inline void SearchInfo::setStopped(const bool s)
{
    stopped = s;
}

inline bool SearchInfo::getQuit() const
{
    return quit;
}

inline void SearchInfo::setQuit(const bool q)
{
    quit = q;
}

} // namespace search

#endif
