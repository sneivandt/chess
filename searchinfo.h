#ifndef SEARCHINFO_H
#define SEARCHINFO_H

#include "utils.h"
#include <iostream>

class SearchInfo
{
    private:

        // Depth of the search
        int depth;

        // Search start time
        long long startTime;

        // Search stop time
        long long stopTime;

        // Nodes searched
        long long nodes;

        // Interupt flag
        bool quit;

        // Stop flag
        bool stopped;

    public:

        // Constructors
        SearchInfo() { reset(); };
        SearchInfo(const int depth): depth(depth), stopTime(-1) {};

        // Reset the search info
        void reset();

        // Calclate the stop time
        void calculateStopTime(const int, const int);

        // Nodes
        long long getNodes() const;
        void incrementNodes();

        // Start time
        long long getStartTime() const;
        void setStartTime();

        // Stop time
        long long getStopTime() const;

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

inline long long SearchInfo::getNodes() const
{
    return nodes;
}

inline void SearchInfo::incrementNodes()
{
    nodes++;
}

inline long long SearchInfo::getStartTime() const
{
    return startTime;
}

inline void SearchInfo::setStartTime()
{
    startTime = utils::getTime();
}

inline long long SearchInfo::getStopTime() const
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

#endif
