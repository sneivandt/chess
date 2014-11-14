#include "init.h"
#include "io.h"
#include "makemove.h"
#include "move.h"
#include "perft.h"
#include "search.h"

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
        if(input == "q") {
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
            searchPosition(pos, info);
        }
        else if(input == "p") {
            std::cout << std::endl;
            perftTest();
            break;
        }
        else {
            try {
                move = parseMove(input, pos);
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
