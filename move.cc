#include "move.h"

std::string Move::getString() const
{
    int fromSquare = SQ64[FROMSQ(value)];
    int toSquare = SQ64[TOSQ(value)];
    int promoted = PROMOTED(value);
    char c[] = { (char)((fromSquare % 8) + 'a'), (char)((fromSquare / 8) + '1'), (char)((toSquare % 8) + 'a'), (char)((toSquare / 8) + '1'), '\0' };
    std::string str(c);
    if(promoted) {
        char promotedChar = PIECE_CHARS[promoted];
        str += promotedChar;
    }
    return str;
}
