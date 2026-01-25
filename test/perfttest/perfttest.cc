#include "perfttest.h"

#include "board/board.h"
#include "board/makemove.h"
#include "board/move.h"
#include "search/movegen.h"
#include "search/movelist.h"
#include "utils/utils.h"

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <vector>

TEST(PerftTest, perft)
{
    test::perft::test();
}

void test::perft::test()
{
    // Get max depth from environment variable, default to 6 if not set
    const int DEFAULT_MAX_DEPTH = 6;
    int maxDepth = DEFAULT_MAX_DEPTH;
    const char* maxDepthEnv = std::getenv("PERFT_MAX_DEPTH");
    if (maxDepthEnv != nullptr) {
        try {
            maxDepth = std::stoi(maxDepthEnv);
            if (maxDepth < 1 || maxDepth > DEFAULT_MAX_DEPTH) {
                maxDepth = DEFAULT_MAX_DEPTH;
            }
        } catch (...) {
            // Invalid input, use default
            maxDepth = DEFAULT_MAX_DEPTH;
        }
    }
    std::cout << "Running perft tests with max depth: " << maxDepth << std::endl;

    board::Board pos;
    std::ifstream input("perft.txt");
    ASSERT_FALSE(input.fail());
    std::string line;
    std::string token;
    std::string delimiter = ";";
    std::clock_t start = std::clock();
    for (int i = 0; std::getline(input, line); i++) {
        bool first = true;
        size_t p = 0;
        do {
            p = line.find(delimiter);
            token = line.substr(0, p);
            if (first) {
                std::cout << utils::getTimestamp() << " P" << (i + 1) << " " << token << std::endl;
                pos.parseFen(token);
            }
            else {
                testPosition(token, pos, maxDepth);
            }
            line.erase(0, p + delimiter.length());
            first = false;
        } while (p != std::string::npos);
    }
    std::cout << "RUNTIME " << (static_cast<double>(std::clock() - start) / static_cast<double>(CLOCKS_PER_SEC)) << "s"
              << std::endl;
}

void test::perft::testPosition(const std::string& token, board::Board& pos, int maxDepth)
{
    int depth = token[1] - '0';
    // Skip tests that exceed max depth
    if (depth > maxDepth) {
        std::cout << utils::getTimestamp() << " D" << depth << " SKIPPED (max depth: " << maxDepth << ")" << std::endl;
        return;
    }
    std::string target = token.substr(token.find(' ') + 1, token.length());
    int64_t nodes = countMoves(depth, pos);
    std::cout << utils::getTimestamp() << " D" << depth << " " << nodes << std::endl;
    ASSERT_TRUE(target.compare(std::to_string(nodes)) == 0);
}

int64_t test::perft::countMoves(const int depth, board::Board& pos)
{
    if (depth == 0) {
        return 1;
    }
    int64_t nodes = 0;
    search::MoveList movelist = search::movegen::generateAll(pos, false);
    for (board::Move move : movelist.getMoves()) {
        if (!board::makemove::move(move, pos)) {
            continue;
        }
        nodes += countMoves(depth - 1, pos);
        board::makemove::undo(pos);
    }
    return nodes;
}