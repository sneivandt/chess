#include "movegen.h"

void MoveList::addMove(const Move move)
{
    moves[count] = move;
    count++;
}

void MoveList::print() const
{
    std::cout << "Move List" << std::endl;
    for(int i = 0; i < count; i++) {
        std::cout << std::setw(3) <<  (i + 1) << " ";
        moves[i].print();
    }
}
