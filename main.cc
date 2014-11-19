#include "board.h"
#include "init.h"
#include "perft.h"
#include "search.h"
#include "searchinfo.h"
#include "uci.h"
#include <string>

int main()
{
    init();
    Board pos;
    Move move;
    std::string input;
    while(true) {
        std::cout << std::endl;
        pos.print();
        std::cout << std::endl;
        std::cout << ((pos.getHistory().size() / 2) + 1) << ": ";
        std::getline(std::cin, input);
        if(input == "uci") {
            uci::loop();
            break;
        }
        else if(input == "q") {
            break;
        }
        else if(input == "n") {
            pos.parseFen(DEFAULT_FEN);
        }
        else if(input.length() > 2 && input.substr(0, 2) == "n ") {
            if(!pos.parseFen(input.substr(2))) {
                std::cout << std::endl;
                std::cout << "ERROR Invalid FEN" << std::endl;
                pos.parseFen(DEFAULT_FEN);
            }
        }
        else if(input == "u") {
            if(pos.getHistory().size() > 0) {
                takeMove(pos);
            }
            else {
                std::cout << std::endl;
                std::cout << "ERROR No move history" << std::endl;
            }
        }
        else if(input == "s") {
            std::cout << std::endl;
            SearchInfo info(7);
            search::searchPosition(pos, info);
        }
        else if(input == "p") {
            std::cout << std::endl;
            perft::test();
            break;
        }
        else {
            try {
                move = io::parseMove(input, pos);
                if(!makeMove(move, pos)) {
                    throw 0;
                }
            }
            catch(int e) {
                std::cout << std::endl;
                std::cout << "ERROR Invalid input" << std::endl;
            }
        }
    }
    std::cout << std::endl;
    return 0;
}
