#include "movegen.h"

void MoveList::addMove(const Move move)
{
    moves.push_back(move);
}

void MoveList::print() const
{
    std::cout << "Move List" << std::endl;
    for(std::vector<Move>::size_type i = 0; i != moves.size(); i++) {
        std::cout << std::setw(3) << (i + 1) << " ";
        std::cout << moves[i].getString() << " ";
    }
    std::cout << std::endl;
}
