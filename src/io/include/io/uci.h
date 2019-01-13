#ifndef UCI_H
#define UCI_H

#include <string>

namespace board {
class Board;
}

namespace search {
class SearchInfo;
}

namespace io { namespace uci {

// Parse go command
void parseGo(const std::string&, search::SearchInfo&, board::Board&);

// Parse a position command
void parsePosition(const std::string&, board::Board&);

// Loop
void loop();

}} // namespace io::uci

#endif
