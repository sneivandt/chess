#pragma once

#include <cstdint>
#include <iosfwd>

namespace board { namespace bitboard {

// Pop a piece from a bitboard
int popBit(uint64_t&);

// Count pieces on a bitboard
int countBits(uint64_t);

// Clear a bitboard
void clearBit(uint64_t&, int);

// Set a bit on a bitboard
void setBit(uint64_t&, int);

// Print
void print(uint64_t);
void print(uint64_t, std::ostream&);

}} // namespace board::bitboard
