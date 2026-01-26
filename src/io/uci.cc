#include "io/uci.h"

#include "board/board.h"
#include "board/makemove.h"
#include "board/move.h"
#include "io/io.h"
#include "search/search.h"
#include "search/searchinfo.h"

#include <cstddef>
#include <iostream>
#include <sstream>

void io::uci::parseGo(const std::string& input, search::SearchInfo& info, board::Board& pos)
{
    int inc = 0;
    int depth = -1;
    int time = -1;
    int movetime = -1;
    int movestogo = -1;
    std::istringstream ss(input);
    std::string key;
    std::string val;
    while (ss >> key) {
        if (key == "depth") {
            ss >> val;
            depth = stoi(val);
        }
        else if (key == "wtime" && pos.getSide() == board::WHITE) {
            ss >> val;
            time = stoi(val);
        }
        else if (key == "btime" && pos.getSide() == board::BLACK) {
            ss >> val;
            time = stoi(val);
        }
        else if (key == "winc" && pos.getSide() == board::WHITE) {
            ss >> val;
            inc = stoi(val);
        }
        else if (key == "binc" && pos.getSide() == board::BLACK) {
            ss >> val;
            inc = stoi(val);
        }
        else if (key == "movetime") {
            ss >> val;
            movetime = stoi(val);
        }
        else if (key == "movestogo") {
            ss >> val;
            movestogo = stoi(val);
        }
    }
    info.setStartTime();
    info.setDepth(depth);
    if (movetime > -1) {
        time = movetime;
        movestogo = 1;
    }
    if (time > -1 && movestogo > 0) {
        time /= movestogo;
    }
    info.calculateStopTime(time, inc);
    search::go(pos, info);
}

void io::uci::parsePosition(const std::string& input, board::Board& pos)
{
    size_t indexStartpos = input.find("startpos");
    size_t indexFen = input.find("fen ");
    size_t indexMoves = input.find("moves ");
    if (indexStartpos == 0) {
        pos.parseFen(board::Board::DEFAULT_FEN);
    }
    else if (indexFen != std::string::npos) {
        if (indexMoves != std::string::npos) {
            // Moves exist, so extract FEN only up to "moves"
            // Length is: indexMoves - (indexFen + 4) - 1 (for space before "moves")
            pos.parseFen(input.substr(indexFen + 4, indexMoves - indexFen - 5));
        }
        else {
            // No moves, so take the rest of the string as FEN
            pos.parseFen(input.substr(indexFen + 4));
        }
    }
    if (indexMoves != std::string::npos) {
        board::Move move;
        std::istringstream ss(input.substr(indexMoves + 6));
        std::string moveString;
        while (ss >> moveString) {
            move = io::parseMove(moveString, pos);
            board::makemove::move(move, pos);
        }
        pos.resetPly();
    }
    pos.print();
}

void io::uci::loop()
{
    std::cout << std::endl;
    std::cout << "id name ACE" << std::endl;
    std::cout << "id author Stuart Neivandt" << std::endl;
    std::cout << "uciok" << std::endl;
    board::Board pos;
    search::SearchInfo info;
    std::string input;
    while (true) {
        std::getline(std::cin, input);
        if (input == "isready") {
            std::cout << "readyok" << std::endl;
        }
        else if (input == "ucinewgame") {
            parsePosition("startposition", pos);
        }
        else if (input.compare(0, 8, "position") == 0) {
            parsePosition(input.substr(9), pos);
        }
        else if (input.compare(0, 2, "go") == 0) {
            parseGo(input, info, pos);
        }
        else if (input == "quit") {
            info.setQuit(true);
            break;
        }
        if (info.getQuit()) {
            break;
        }
    }
}
