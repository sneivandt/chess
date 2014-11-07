#ifndef SEARCHINFO_H
#define SEARCHINFO_H

#include "utils.h"

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

    public:

        // Constructor
        SearchInfo(const int depth): depth(depth) {};

        // Reset info
        void reset();

        // Increment nodes
        void incrementNodes() { nodes++; };

        // Get depth
        int getDepth() const { return depth; };

        // Get nodes
        long long getNodes() const { return nodes; };
};

#endif
