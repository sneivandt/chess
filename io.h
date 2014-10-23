#ifndef IO_H
#define IO_H

#include "board.h"
#include "move.h"
#include "movegen.h"
#include "movelist.h"
#include <string>

// Parse an input move and throw if the input is invalid
Move parseMove(std::string, Board&) throw(int);

#endif
