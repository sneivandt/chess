#include "perft.h"

void perftTest()
{
    Board pos;
    std::ifstream input(TEST_SPECS);
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
                std::cout << getTimestamp() << " P" << (i + 1) << " " << token << std::endl;
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

void testPosition(const std::string token, Board& pos)
{
    int depth = token[1] - '0';
    std::string target = token.substr(token.find(" ") + 1, token.length());
    long long nodes = testPositionInner(depth, pos);
    std::cout << getTimestamp() << " D" << depth << " " << nodes << std::endl;
    assert(target.compare(std::to_string(nodes)) == 0);
}

long long testPositionInner(const int depth, Board pos)
{
    if(depth == 0) {
        return 1;
    }
    long long nodes = 0;
    MoveList moves = generateAllMoves(pos);
    for(int i = 0; i < moves.getCount(); i++) {
        Move move = moves.getMoves()[i];
        if(!makeMove(move, pos)) {
            continue;
        }
        nodes += testPositionInner(depth - 1, pos);
        takeMove(pos);
    }
    return nodes;
}
