#include "perft.h"

void perft::test()
{
    Board pos;
    std::ifstream input(TEST_FILE);
    std::string line;
    std::string token;
    std::string delimiter = ";";
    std::clock_t start = std::clock();
    for(int i = 0; std::getline(input, line); i++) {
        bool first = true;
        size_t p = 0;
        do {
            p = line.find(delimiter);
            token = line.substr(0, p);
            if(first) {
                std::cout << utils::getTimestamp() << " P" << (i + 1) << " " << token << std::endl;
                pos.parseFen(token);
            }
            else {
                testPosition(token, pos);
            }
            line.erase(0, p + delimiter.length());
            first = false;
        }
        while(p != std::string::npos);
    }
    std::cout << "RUNTIME " << ((std::clock() - start)/(float)CLOCKS_PER_SEC) << "s" << std::endl;
}

void perft::testPosition(const std::string token, Board &pos)
{
    int depth = token[1] - '0';
    std::string target = token.substr(token.find(" ") + 1, token.length());
    long long nodes = countMoves(depth, pos);
    std::cout << utils::getTimestamp() << " D" << depth << " " << nodes << std::endl;
    assert(target.compare(std::to_string(nodes)) == 0);
}

long long perft::countMoves(const int depth, Board &pos)
{
    if(depth == 0) {
        return 1;
    }
    long long nodes = 0;
    MoveList movelist = movegen::generateAll(pos, false);
    std::vector<Move> moves = movelist.getMoves();
    for(unsigned int i = 0; i < moves.size(); i++) {
        Move move = moves[i];
        if(!makemove::move(move, pos)) {
            continue;
        }
        nodes += countMoves(depth - 1, pos);
        makemove::undo(pos);
    }
    return nodes;
}
