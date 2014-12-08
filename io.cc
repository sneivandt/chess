#include "io.h"

Move io::parseMove(const std::string input, Board &pos)
{
    if(input.length() > 3) {
        int from = Board::FR2SQ((int)(input.at(0) - 'a'), (int)(input.at(1) - '1'));
        int to = Board::FR2SQ((int)(input[2] - 'a'), (int)(input[3] - '1'));
        int promoted = 0;
        MoveList movelist = movegen::generateAll(pos,false);
        for(Move move : movelist.getMoves()) {
            if(Move::FROMSQ(move.getValue()) == from && Move::TOSQ(move.getValue()) == to) {
                promoted = Move::PROMOTED(move.getValue());
                if(promoted == EMPTY) {
                    return move;
                }
                else if(input.length() > 4) {
                    if(Board::PIECE_NO_TEAM[promoted] == WR && input.at(4) == 'r') {
                        return move;
                    }
                    else if(Board::PIECE_NO_TEAM[promoted] == WB && input.at(4) == 'b') {
                        return move;
                    }
                    else if(Board::PIECE_NO_TEAM[promoted] == WQ && input.at(4) == 'q') {
                        return move;
                    }
                    else if(Board::PIECE_NO_TEAM[promoted] == WN && input.at(4) == 'n') {
                        return move;
                    }
                }
            }
        }
    }
    throw 0;
}
