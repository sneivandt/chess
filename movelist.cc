#include "movelist.h"

void MoveList::print() const
{
    std::cout << "Move List" << std::endl;
    for(unsigned int i = 0; i < moves.size(); i++) {
        std::cout << std::setw(3) << (i + 1) << " ";
        std::cout << moves[i].getString() << " ";
    }
    std::cout << std::endl;
}
