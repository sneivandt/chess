#ifndef UCI_H
#define UCI_H

#include "board.h"
#include "io.h"
#include "makemove.h"
#include "search.h"
#include "searchinfo.h"
#include <iostream>
#include <sstream>
#include <string>

namespace uci
{

// Parse go command
void parseGo(const std::string, SearchInfo&, Board&);

// Parse a position command
void parsePosition(const std::string, Board&);

// Loop
void loop();

}

#endif
