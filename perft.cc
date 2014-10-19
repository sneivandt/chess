#include "perft.h"

void Perft::test()
{
    std::ifstream input(TEST_SPECS);
    std::string delimiter = ";";
    std::string line;
    std::string token;
    Board pos;
    for(int i = 0; std::getline(input, line); i++) {
        bool first = true;
        size_t p = 0;
        do {
            p = line.find(delimiter);
            token = line.substr(0, p);
            if(first) {
                std::cout << token << std::endl;
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
    std::cout << "DONE: " << totalPositions << std::endl;
}

void Perft::testPosition(const std::string token, Board& pos)
{
    nodes = 0;
    int depth = token[1] - '0';
    std::string target = token.substr(token.find(" ") + 1, token.length());
    testPositionInner(depth, pos);
    std::cout << "D" << depth << " " << nodes << std::endl;
    assert(target.compare(std::to_string(nodes)) == 0);
}

void Perft::testPositionInner(const int depth, Board pos)
{
    totalPositions++;
    if(depth == 0) {
        nodes++;
        return;
    }
    MoveList moves = generateAllMoves(pos);
    for(int i = 0; i < moves.getCount(); i++) {
        Move move = moves.getMoves()[i];
        if(!makeMove(move, pos)) {
            continue;
        }
        testPositionInner(depth - 1, pos);
        takeMove(pos);
    }
}
