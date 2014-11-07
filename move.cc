#include "move.h"

inline std::string squareName(const int square)
{
    int s = SQ64[square]; char c[] = { (char)((s % 8) + 'a'), (char)((s / 8) + '1'), '\0' };
    std::string name(c);
    return name;
}

std::string Move::getString() const
{
    int fromSquare = FROMSQ(value);
    int toSquare = TOSQ(value);
    int promoted = PROMOTED(value);
    std::string str = squareName(fromSquare) + squareName(toSquare);
    if(promoted) {
        char promotedChar = PIECE_CHARS[promoted];
        str += promotedChar;
    }
    return str;
}

