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
        std::cout << ((pos.getPly() / 2) + 1) << ": ";
        std::cin >> input;
        if(input == "exit") {
            break;
        }
        else if(input == "new") {
            pos.parseFen(DEFAULT_FEN);
        }
        else if(input == "undo") {
            if(pos.getPly() == 0) {
                std::cout << std::endl;
                std::cout << "WARNING: At game start" << std::endl;
            }
            else {
                takeMove(pos);
            }
        }
        else if(input == "test") {
            std::cout << std::endl;
            perftTest();
            break;
        }
        else if(input == "help" || input == "h") {
            std::cout << std::endl;
            std::cout << "new  - New game" << std::endl;
            std::cout << "undo - Undo move" << std::endl;
            std::cout << "test - Perft test" << std::endl;
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
                std::cout << "ERROR: Invalid input" << std::endl;
            }
        }
    }
    std::cout << std::endl;
    return 0;
}
