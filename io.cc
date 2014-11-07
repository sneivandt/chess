#include "io.h"

Move parseMove(const std::string input, Board& pos) throw(int)
{
    if(input.length() > 3) {
        int from = FR2SQ((int)(input.at(0) - 'a'), (int)(input.at(1) - '1'));
        int to = FR2SQ((int)(input[2] - 'a'), (int)(input[3] - '1'));
        int promoted = 0;
        MoveList movelist = generateAllMoves(pos);
        std::vector<Move> moves = movelist.getMoves();
        Move move;
        for(std::vector<Move>::size_type i = 0; i != moves.size(); i++) {
            move = moves[i];
            if(FROMSQ(move.getValue()) == from && TOSQ(move.getValue()) == to) {
                promoted = PROMOTED(move.getValue());
                if(promoted == EMPTY) {
                    return move;
                }
                else if(input.length() > 4) {
                    if(PIECE_ROOK[promoted] && input.at(4) == 'r') {
                        return move;
                    }
                    else if(PIECE_BISHOP[promoted] && input.at(4) == 'b') {
                        return move;
                    }
                    else if(PIECE_QUEEN[promoted] && input.at(4) == 'q') {
                        return move;
                    }
                    else if(PIECE_KNIGHT[promoted] && input.at(4) == 'n') {
                        return move;
                    }
                }
            }
        }
    }
    throw 0;
}
