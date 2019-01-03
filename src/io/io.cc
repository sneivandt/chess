#include "io/io.h"

board::Move io::parseMove(const std::string& input, board::Board& pos)
{
    if (input.length() > 3) {
        int from = board::Board::FR2SQ(input.at(0) - 'a', input.at(1) - '1');
        int to = board::Board::FR2SQ(input[2] - 'a', input[3] - '1');
        int promoted = 0;
        search::MoveList movelist = search::movegen::generateAll(pos, false);
        for (board::Move move : movelist.getMoves()) {
            if (board::Move::FROMSQ(move.getValue()) == from && board::Move::TOSQ(move.getValue()) == to) {
                promoted = board::Move::PROMOTED(move.getValue());
                if (promoted == board::EMPTY) {
                    return move;
                }
                if (input.length() > 4) {
                    if (board::Board::PIECE_NO_TEAM[promoted] == board::WR && input.at(4) == 'r') {
                        return move;
                    }
                    if (board::Board::PIECE_NO_TEAM[promoted] == board::WB && input.at(4) == 'b') {
                        return move;
                    }
                    if (board::Board::PIECE_NO_TEAM[promoted] == board::WQ && input.at(4) == 'q') {
                        return move;
                    }
                    if (board::Board::PIECE_NO_TEAM[promoted] == board::WN && input.at(4) == 'n') {
                        return move;
                    }
                }
            }
        }
    }
    throw 0;
}
