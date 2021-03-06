#ifndef SEARCH_H
#define SEARCH_H

#include <limits>

namespace board {
class Board;
}

namespace search {

class SearchInfo;
class PVTable;

// Positive infinity
const int POS_INFINITY = std::numeric_limits<int>::max() - 1;

// Negative infinity
const int NEG_INFINITY = std::numeric_limits<int>::min() + 1;

// Check stopping conditions
void checkup(SearchInfo&);

// Check if a board is a repetition
bool isRepetition(board::Board&);

// Reset search data
void reset(board::Board&, SearchInfo&);

// Negamax
int negamax(int, int, int, board::Board&, SearchInfo&, PVTable&);

// Iterative deepening
void go(board::Board&, SearchInfo&);

} // namespace search

#endif
