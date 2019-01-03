#ifndef IO_H
#define IO_H

#include "board/board.h"
#include "board/move.h"
#include "search/movegen.h"
#include "search/movelist.h"

#include <string>
#include <vector>

namespace io {

// Parse an input move
board::Move parseMove(const std::string&, board::Board&);

} // namespace io

#endif
