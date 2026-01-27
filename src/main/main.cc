#include "board/board.h"
#include "board/evaluate.h"
#include "board/makemove.h"
#include "board/move.h"
#include "io/io.h"
#include "io/uci.h"
#include "search/movegen.h"
#include "search/search.h"
#include "search/searchinfo.h"
#include "utils/exceptions.h"

#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>

int main()
{
    board::Board::INIT();
    search::movegen::INIT();
    board::Board pos;
    board::Move move;
    std::string input;
    bool isInteractive = isatty(fileno(stdin));

    while (true) {
        std::cout << std::endl;
        pos.print();
        std::cout << std::endl;
        std::cout << ((pos.getHistory().size() / 2) + 1) << ": ";
        if (!std::getline(std::cin, input)) {
            break;
        }
        if (input == "uci") {
            io::uci::loop();
            break;
        }
        if (input == "q") {
            break;
        }
        if (input == "n") {
            pos.parseFen(board::Board::DEFAULT_FEN);
        }
        else if (input.length() > 2 && input.substr(0, 2) == "n ") {
            if (!pos.parseFen(input.substr(2))) {
                std::cout << std::endl;
                std::cout << "ERROR Invalid FEN" << std::endl;
                if (!isInteractive) {
                    return 1;
                }
                pos.parseFen(board::Board::DEFAULT_FEN);
            }
        }
        else if (input == "u") {
            if (!pos.getHistory().empty()) {
                board::makemove::undo(pos);
            }
        }
        else if (input == "s") {
            std::cout << std::endl;
            search::SearchInfo info(-1);
            search::go(pos, info);
        }
        else if (input == "e") {
            std::cout << std::endl;
            std::cout << "Evaluation: " << board::evaluate::score(pos) << std::endl;
        }
        else {
            try {
                move = io::parseMove(input, pos);
                if (!board::makemove::move(move, pos)) {
                    throw utils::AceException("Invalid move");
                }
            }
            catch (const std::exception& e) {
                std::cout << std::endl;
                std::cout << "ERROR " << e.what() << std::endl;
                if (!isInteractive) {
                    return 1;
                }
            }
        }
    }
    std::cout << std::endl;
    return 0;
}
