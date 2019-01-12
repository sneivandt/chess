#include "perft/perft.h"

int main() {
    test::perft::test();
}

void test::perft::test()
{
    board::Board pos;
    std::ifstream input("perft.txt");
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

void test::perft::testPosition(const std::string token, board::Board &pos)
{
    int depth = token[1] - '0';
    std::string target = token.substr(token.find(" ") + 1, token.length());
    int64_t nodes = countMoves(depth, pos);
    std::cout << utils::getTimestamp() << " D" << depth << " " << nodes << std::endl;
    assert(target.compare(std::to_string(nodes)) == 0);
}

int64_t test::perft::countMoves(const int depth, board::Board &pos)
{
    if(depth == 0) {
        return 1;
    }
    int64_t nodes = 0;
    search::MoveList movelist = search::movegen::generateAll(pos, false);
    for(board::Move move : movelist.getMoves()) {
        if(!board::makemove::move(move, pos)) {
            continue;
        }
        nodes += countMoves(depth - 1, pos);
        board::makemove::undo(pos);
    }
    return nodes;
}