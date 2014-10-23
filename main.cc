#include "init.h"
#include "io.h"
#include "move.h"
#include "perft.h"
#include "makemove.h"
#include <iostream>

int main()
{
    init();
    Board pos;
    Move move;
    std::string input;
    while(true) {
        pos.print();
        std::cout << ((pos.getPly() / 2) + 1) << ": ";
        std::cin >> input;
        if(input == "exit") {
            return 0;
        }
        else if(input == "new") {
            pos.parseFen(DEFAULT_FEN);
        }
        else if(input == "undo" && pos.getPly() > 0) {
            takeMove(pos);
        }
        else if(input == "test") {
            perftTest();
            return 0;
        }
        else if(input == "help") {
            std::cout << std::endl;
            std::cout << "new    Start a new game" << std::endl;
            std::cout << "undo   Take back a move" << std::endl;
            std::cout << "test   Run the perft test" << std::endl;
        }
        else {
            try {
                move = parseMove(input, pos);
            }
            catch(int e) {
                std::cout << std::endl << "ERROR" << std::endl;
                continue;
            }
            if(!makeMove(move, pos)) {
                std::cout << std::endl << "ERROR" << std::endl;
            }
        }
    }
    return 0;
}
