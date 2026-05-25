#include "search/movelist.h"

#include <iomanip>
#include <iostream>
#include <memory>
#include <string>

void search::MoveList::print() const
{
    print(std::cout);
}

void search::MoveList::print(std::ostream& output) const
{
    output << "Move List" << std::endl;
    for (unsigned int i = 0; i < moves.size(); i++) {
        output << std::setw(3) << (i + 1) << " ";
        output << moves[i].getString() << " ";
    }
    output << std::endl;
}
