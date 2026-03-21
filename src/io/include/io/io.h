#pragma once

#include "board/move.h"

#include <optional>
#include <string>

namespace board {
class Board;
}

namespace io {

// Parse an input move (throws AceException on invalid input)
board::Move parseMove(const std::string&, board::Board&);

// Try to parse an input move (returns std::nullopt on invalid input)
std::optional<board::Move> tryParseMove(const std::string&, board::Board&);

} // namespace io
