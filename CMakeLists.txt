cmake_minimum_required(VERSION 3.6)

# Project

project(chess VERSION 1.0.0)

# Options

option(CLANG_TIDY "Build with Clang-Tidy" OFF)
option(INCLUDE_WHAT_YOU_USE "Build with Include What You Use" OFF)

# Global Flags

set(CMAKE_CXX_STANDARD 17)

if(MSVC)
  include(cmake/msvc-runtime.cmake)
endif()

# Source

add_subdirectory(src)

# Test

enable_testing()

set(INSTALL_GTEST OFF CACHE INTERNAL "Do not install gtest")
set(BUILD_GMOCK OFF CACHE INTERNAL "Do not build gmock")
add_subdirectory(extern/googletest)

add_subdirectory(test)

# Tools

include(cmake/clang-format.cmake)

if(CLANG_TIDY)
    include(cmake/clang-tidy.cmake)
endif()

if(INCLUDE_WHAT_YOU_USE)
    include(cmake/include-what-you-use.cmake)
endif()
