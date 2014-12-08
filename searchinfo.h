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
        long startTime;

        // Search stop time
        long stopTime;

        // Nodes searched
        long long nodes;

        // Interupt flag
        bool quit;

        // Stop flag
        bool stopped;

        // Fail high
        long long failHigh;

        // Fail high first
        long long failHighFirst;

    public:

        // Constructors
        SearchInfo() { reset(); };
        SearchInfo(const int depth): depth(depth), stopTime(-1) {};

        // Reset the search info
        void reset();

        // Calclate the stop time
        void calculateStopTime(const int, const int);

        // Get the ordering efficiency
        float getOrdering() const;

        // Nodes
        long long getNodes() const;
        void incrementNodes();

        // Start time
        long getStartTime() const;
        void setStartTime();

        // Stop time
        long getStopTime() const;

        // Depth
        int getDepth() const;
        void setDepth(const int);

        // Stopped
        bool getStopped() const;
        void setStopped(const bool);

        // Quit
        bool getQuit() const;
        void setQuit(const bool);

        // Fail high
        void incrementFailHigh();
        void incrementFailHighFirst();
};

inline float SearchInfo::getOrdering() const
{
    return failHigh == 0 ? 0 : (float)failHighFirst/failHigh;
}

inline long long SearchInfo::getNodes() const
{
    return nodes;
}

inline void SearchInfo::incrementNodes()
{
    nodes++;
}

inline long SearchInfo::getStartTime() const
{
    return startTime;
}

inline void SearchInfo::setStartTime()
{
    startTime = utils::getTime();
}

inline long SearchInfo::getStopTime() const
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

inline void SearchInfo::incrementFailHigh()
{
    failHigh++;
}

inline void SearchInfo::incrementFailHighFirst()
{
    failHighFirst++;
}

#endif
