#ifndef UTILS_H
#define UTILS_H

#include <cstdint>
#include <string>

namespace utils {

// Get a timestamp
std::string getTimestamp();

// Get time in milliseconds
int64_t getTime();

// Check if input is waiting in stdin
bool inputWaiting();

} // namespace utils

#endif
