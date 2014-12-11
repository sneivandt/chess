#ifndef INIT_H
#define INIT_H

#include "board.h"
#include "movegen.h"

namespace init
{

// Initialize all
void all();

}

inline void init::all()
{
    Board::INIT();
    movegen::INIT();
}

#endif
