#ifndef IO_H
#define IO_H

#include "board.h"
#include "move.h"
#include "movegen.h"
#include "movelist.h"
#include <string>
#include <vector>

namespace io
{

// Parse an input move
Move parseMove(std::string, Board&);

}

#endif
