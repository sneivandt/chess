#ifndef SEARCHINFO_H
#define SEARCHINFO_H

#include "utils.h"
#include <iostream>

class SearchInfo
{
    private:

        // Search start time
        int startTime;

        // Search stop time
        int stopTime;

        // Depth of the search
        int depth;

        // Maximum depth to search
        int depthSet;

        // Maximum time to search
        int timeSet;

        // Moves to go
        int movesToGo;

        // Infinate search flag
        bool infinate;

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

        // Searchinfo with a given depth limit
        SearchInfo(const int depth): depth(depth) {};

        // Reset the search info
        void reset();

        // Get the ordering efficiency
        float getOrdering() const { return failHigh == 0 ? 0 : (float)failHighFirst/failHigh; };

        // Increment nodes
        void incrementNodes() { nodes++; };

        // Increment fail high
        void incrementFailHigh() { failHigh++; };

        // Increment fail high first
        void incrementFailHighFirst() { failHighFirst++; };

        // Get the depth
        int getDepth() const { return depth; };

        // Get the nodes
        long long getNodes() const { return nodes; };
};

#endif
