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

        // Default constructor
        SearchInfo() { reset(); };

        // Preset search info
        SearchInfo(const int depth): depth(depth) {};

        // Reset the search info
        void reset();

        // Calclate the stop time
        void calculateStopTime(const int, const int);

        // Get the ordering efficiency
        float getOrdering() const { return failHigh == 0 ? 0 : (float)failHighFirst/failHigh; };

        // Increment nodes
        void incrementNodes() { nodes++; };

        // Increment fail high
        void incrementFailHigh() { failHigh++; };

        // Increment fail high first
        void incrementFailHighFirst() { failHighFirst++; };

        // Get the nodes
        long long getNodes() const { return nodes; };

        // Get the stoptime
        long getStopTime() const { return stopTime; };

        // Get the start time
        long getStartTime() const { return startTime; };

        // Set the start time
        void setStartTime() { startTime = utils::getTime(); };

        // Get the depth
        int getDepth() const { return depth; };

        // Set depth
        void setDepth(const int dep) { depth = dep; };

        // Set stopped
        void setStopped(const bool stop) { stopped = stop; };

        // Get stopped
        bool getStopped() const { return stopped; };

        // Set quit
        void setQuit(const bool val) { quit = val; };

        // Get quit
        bool getQuit() const { return quit; };
};

#endif
