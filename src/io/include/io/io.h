#ifndef IO_H
#define IO_H

#include "board/move.h"

#include <string>

namespace board {
class Board;
}

namespace io {

// Parse an input move
board::Move parseMove(const std::string&, board::Board&);

} // namespace io

#endif
