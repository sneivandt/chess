#include "uci.h"

void uci::parseGo(const std::string input, SearchInfo &info, Board &pos)
{
    int inc = 0;
    int depth = -1;
    int time = -1;
    int movetime = -1;
    int movestogo = -1;
    std::istringstream ss(input);
    std::string key;
    std::string val;
    while(ss >> key) {
        if(key == "depth") {
            ss >> val;
            depth = stoi(val);
        }
        else if(key == "wtime" && pos.getSide() == WHITE) {
            ss >> val;
            time = stoi(val);
        }
        else if(key == "btime" && pos.getSide() == BLACK) {
            ss >> val;
            time = stoi(val);
        }
        else if(key == "winc" && pos.getSide() == WHITE) {
            ss >> val;
            inc = stoi(val);
        }
        else if(key == "binc" && pos.getSide() == BLACK) {
            ss >> val;
            inc = stoi(val);
        }
        else if(key == "movetime") {
            ss >> val;
            movetime = stoi(val);
        }
        else if(key == "movestogo") {
            ss >> val;
            movestogo = stoi(val);
        }
    }
    info.setStartTime();
    info.setDepth(depth);
    if(movetime > -1) {
        time = movetime;
        movestogo = 1;
    }
    if(time > -1 && movestogo > 0) {
        time /= movestogo;
    }
    info.calculateStopTime(time, inc);
    search::go(pos, info);
}

void uci::parsePosition(const std::string input, Board &pos)
{
    int indexStartpos = input.find("startpos");
    int indexFen = input.find("fen ");
    int indexMoves = input.find("moves ");
    if(indexStartpos == 0) {
        pos.parseFen(DEFAULT_FEN);
    }
    else if(indexFen > -1) {
        if(indexMoves == -1) {
            pos.parseFen(input.substr(indexFen + 4));
        }
        else {
            pos.parseFen(input.substr(indexFen + 4, indexMoves - 5));
        }
    }
    if(indexMoves > -1) {
        Move move;
        std::istringstream ss(input.substr(indexMoves + 6));
        std::string moveString;
        while(ss >> moveString) {
            move = io::parseMove(moveString, pos);
            makemove::move(move, pos);
        }
        pos.resetPly();
    }
    pos.print();
}

void uci::loop()
{
    std::cout << std::endl;
    std::cout << "id name ACE" << std::endl;
    std::cout << "id author Stuart Neivandt" << std::endl;
    std::cout << "uciok" << std::endl;
    Board pos;
    SearchInfo info;
    std::string input;
    while(true) {
        std::getline(std::cin, input);
        if(input == "isready") {
            std::cout << "readyok" << std::endl;
        }
        else if(input == "ucinewgame") {
            parsePosition("startposition", pos);
        }
        else if(input.find("position") == 0) {
            parsePosition(input.substr(9), pos);
        }
        else if(input.find("go") == 0) {
            parseGo(input, info, pos);
        }
        else if(input == "quit") {
            info.setQuit(true);
            break;
        }
        if(info.getQuit()) {
            break;
        }
    }
}
