#ifndef UCI_H
#define UCI_H

#include "board/board.h"
#include "board/makemove.h"
#include "io/io.h"
#include "search/search.h"
#include "search/searchinfo.h"

#include <iostream>
#include <sstream>
#include <string>

namespace io {
namespace uci {

// Parse go command
void parseGo(const std::string&, search::SearchInfo&, board::Board&);

// Parse a position command
void parsePosition(const std::string&, board::Board&);

// Loop
void loop();

} // namespace uci
} // namespace io

#endif
