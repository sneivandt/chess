#pragma once

#include "board/board.h"

#include <array>
#include <cstddef>

namespace search {

inline constexpr int MAX_SEARCH_PLY = 128;
inline constexpr int KILLER_MOVE_SLOTS = 2;

// Safe index cast for array access
constexpr std::size_t idx(int i) noexcept
{
    return static_cast<std::size_t>(i);
}

// Search state separated from board position
// Contains move ordering heuristics used during search
struct SearchState
{
    // History heuristic: indexed by [piece][to_square]
    std::array<std::array<int, board::BOARD_SQUARE_NUM>, board::PIECE_TYPE_NUM> history{};

    // Killer moves: indexed by [slot][ply]
    std::array<std::array<int, MAX_SEARCH_PLY>, KILLER_MOVE_SLOTS> killers{};

    int getKiller(const int num, const int ply) const noexcept
    {
        if (ply >= 0 && ply < MAX_SEARCH_PLY && num >= 0 && num < KILLER_MOVE_SLOTS) {
            return killers[idx(num)][idx(ply)];
        }
        return 0;
    }

    void addKiller(const int move, const int ply) noexcept
    {
        if (ply >= 0 && ply < MAX_SEARCH_PLY) {
            killers[1][idx(ply)] = killers[0][idx(ply)];
            killers[0][idx(ply)] = move;
        }
    }

    int getHistory(const int piece, const int square) const noexcept
    {
        return history[idx(piece)][idx(square)];
    }

    void incrementHistory(const int from, const int to, const int depth, const int fromPiece) noexcept
    {
        if (from >= 0 && from < board::BOARD_SQUARE_NUM && to >= 0 && to < board::BOARD_SQUARE_NUM && fromPiece >= 0 &&
            fromPiece < board::PIECE_TYPE_NUM) {
            history[idx(fromPiece)][idx(to)] += depth;
        }
    }

    void clear() noexcept
    {
        history = {};
        killers = {};
    }
};

} // namespace search
