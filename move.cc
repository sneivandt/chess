#include "move.h"

inline std::string squareName(const int square)
{
    int s = SQ64[square]; char c[] = { (char)((s % 8) + 'a'), (char)((s / 8) + '1'), '\0' };
    std::string name(c);
    return name;
}

void Move::print() const
{
    int fromSquare = FROMSQ(value);
    int toSquare = TOSQ(value);
    int promoted = PROMOTED(value);
    std::cout << squareName(fromSquare) << squareName(toSquare);
    if(promoted) {
        char promotedChar = "  nbrq  nbrq "[promoted];
        std::cout << promotedChar;
    }
    std::cout << std::endl;
}

