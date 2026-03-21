#pragma once

#include <array>
#include <cstddef>

namespace search {

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
    std::array<std::array<int, 120>, 13> history{};

    // Killer moves: indexed by [slot][ply]
    std::array<std::array<int, 128>, 2> killers{};

    int getKiller(const int num, const int ply) const noexcept
    {
        if (ply >= 0 && ply < 128 && num >= 0 && num < 2) {
            return killers[idx(num)][idx(ply)];
        }
        return 0;
    }

    void addKiller(const int move, const int ply) noexcept
    {
        if (ply >= 0 && ply < 128) {
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
        if (from >= 0 && from < 120 && to >= 0 && to < 120 && fromPiece >= 0 && fromPiece < 13) {
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
