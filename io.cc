#include "io.h"

Move io::parseMove(const std::string input, Board &pos)
{
    if(input.length() > 3) {
        int from = FR2SQ((int)(input.at(0) - 'a'), (int)(input.at(1) - '1'));
        int to = FR2SQ((int)(input[2] - 'a'), (int)(input[3] - '1'));
        int promoted = 0;
        MoveList movelist = movegen::generateAll(pos,false);
        std::vector<Move> moves = movelist.getMoves();
        Move move;
        for(unsigned int i = 0; i < moves.size(); i++) {
            move = moves[i];
            if(FROMSQ(move.getValue()) == from && TOSQ(move.getValue()) == to) {
                promoted = PROMOTED(move.getValue());
                if(promoted == EMPTY) {
                    return move;
                }
                else if(input.length() > 4) {
                    if(PIECE_NO_TEAM[promoted] == WR && input.at(4) == 'r') {
                        return move;
                    }
                    else if(PIECE_NO_TEAM[promoted] == WB && input.at(4) == 'b') {
                        return move;
                    }
                    else if(PIECE_NO_TEAM[promoted] == WQ && input.at(4) == 'q') {
                        return move;
                    }
                    else if(PIECE_NO_TEAM[promoted] == WN && input.at(4) == 'n') {
                        return move;
                    }
                }
            }
        }
    }
    throw 0;
}
