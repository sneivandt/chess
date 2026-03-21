#include "board/move.h"

#include "board/board.h"

std::string board::Move::getString() const
{
    int fromSquare = Board::SQ64[idx(FROMSQ(value))];
    int toSquare = Board::SQ64[idx(TOSQ(value))];
    int promoted = PROMOTED(value);
    char c[] = {static_cast<char>((fromSquare % 8) + 'a'), static_cast<char>((fromSquare / 8) + '1'),
                static_cast<char>((toSquare % 8) + 'a'), static_cast<char>((toSquare / 8) + '1'), '\0'};
    std::string str(static_cast<char*>(c));
    if (promoted) {
        char promotedChar = Board::PIECE_CHARS[idx(promoted)];
        str += promotedChar;
    }
    return str;
}
