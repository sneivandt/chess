#include "move.h"

std::string Move::getString() const
{
    int fromSquare = Board::SQ64[FROMSQ(value)];
    int toSquare = Board::SQ64[TOSQ(value)];
    int promoted = PROMOTED(value);
    char c[] = { (char)((fromSquare % 8) + 'a'), (char)((fromSquare / 8) + '1'), (char)((toSquare % 8) + 'a'), (char)((toSquare / 8) + '1'), '\0' };
    std::string str(c);
    if(promoted) {
        char promotedChar = Board::PIECE_CHARS[promoted];
        str += promotedChar;
    }
    return str;
}
