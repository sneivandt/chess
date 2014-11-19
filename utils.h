#ifndef UTILS_H
#define UTILS_H

#ifdef WIN32
#define NOMINMAX
#include "Windows.h"
#endif

#include <chrono>
#include <ctime>
#include <string>

namespace utils
{

// Get a timestamp
std::string getTimestamp();

// Get time in milliseconds
long getTime();

// Check if input is waiting in stdin
bool inputWaiting();

}

#endif
