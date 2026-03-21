#pragma once

#include <stdexcept>
#include <string>

namespace utils {

class AceException : public std::runtime_error
{
  public:
    explicit AceException(const std::string& message) : std::runtime_error(message)
    {
    }
};

} // namespace utils
