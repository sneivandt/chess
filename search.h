#ifndef SEARCH_H
#define SEARCH_H

#include "board.h"
#include "evaluate.h"
#include "movegen.h"
#include "movelist.h"
#include "pvtable.h"
#include "searchinfo.h"
#include "undo.h"
#include <algorithm>
#include <limits>
#include <vector>

namespace search
{

// Positive infinity
const int POS_INFINITY = std::numeric_limits<int>::max() - 1;

// Negative infinity
const int NEG_INFINITY = std::numeric_limits<int>::min() + 1;

// Check stopping conditions
void checkup(SearchInfo&);

// Check if a board is a repetition
bool isRepetition(Board&);

// Reset search data
void reset(Board&, SearchInfo&);

// Negamax
int negamax(int, int, int, Board&, SearchInfo&, PVTable&);

// Iterative deepening
void go(Board&, SearchInfo&);

}

#endif
