#ifndef SEARCH_H
#define SEARCH_H

#include "board.h"
#include "evaluate.h"
#include "movegen.h"
#include "movelist.h"
#include "pvtable.h"
#include "searchinfo.h"
#include "undo.h"
#include <limits>
#include <vector>

// Positive infinity
const int POS_INFINITY = std::numeric_limits<int>::max() - 1;

// Negative infinity
const int NEG_INFINITY = std::numeric_limits<int>::min() + 1;

// Mate score
const int MATE = 100000;

// Check stopping conditions
void checkUp();

// Check if a board is a repetition
bool isRepetition(Board&);

// Clear search data
void clearForSearch(Board&, SearchInfo&);

// Quiesence
int quiescence(const int, const int, Board&, SearchInfo&);

// Alpha beta
int alphaBeta(int, int, const int, Board&, SearchInfo&, PVTable&);

// Iterative deepening
void searchPosition(Board&, SearchInfo&);

#endif
