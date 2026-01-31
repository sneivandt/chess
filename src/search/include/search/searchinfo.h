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
    SearchInfo(const int d) : depth(d), stopTime(-1) {};

    // Copy semantics
    SearchInfo(const SearchInfo&) = default;
    SearchInfo& operator=(const SearchInfo&) = default;

    // Move semantics
    SearchInfo(SearchInfo&&) noexcept = default;
    SearchInfo& operator=(SearchInfo&&) noexcept = default;

    // Reset the search info
    void reset();

    // Calclate the stop time
    void calculateStopTime(const int, const int);

    // Nodes
    int64_t getNodes() const noexcept;
    void incrementNodes() noexcept;

    // Start time
    int64_t getStartTime() const noexcept;
    void setStartTime();

    // Stop time
    int64_t getStopTime() const noexcept;

    // Depth
    int getDepth() const noexcept;
    void setDepth(const int) noexcept;

    // Stopped
    bool getStopped() const noexcept;
    void setStopped(const bool) noexcept;

    // Quit
    bool getQuit() const noexcept;
    void setQuit(const bool) noexcept;
};

inline int64_t SearchInfo::getNodes() const noexcept
{
    return nodes;
}

inline void SearchInfo::incrementNodes() noexcept
{
    nodes++;
}

inline int64_t SearchInfo::getStartTime() const noexcept
{
    return startTime;
}

inline void SearchInfo::setStartTime()
{
    startTime = utils::getTime();
}

inline int64_t SearchInfo::getStopTime() const noexcept
{
    return stopTime;
}

inline int SearchInfo::getDepth() const noexcept
{
    return depth;
}

inline void SearchInfo::setDepth(const int d) noexcept
{
    depth = d;
}

inline bool SearchInfo::getStopped() const noexcept
{
    return stopped;
}

inline void SearchInfo::setStopped(const bool s) noexcept
{
    stopped = s;
}

inline bool SearchInfo::getQuit() const noexcept
{
    return quit;
}

inline void SearchInfo::setQuit(const bool q) noexcept
{
    quit = q;
}

} // namespace search

#endif
