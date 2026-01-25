cmake_minimum_required(VERSION 3.6)

# Project

project(chess VERSION 1.0.0)

# Options

option(CLANG_TIDY "Build with Clang-Tidy" OFF)
option(INCLUDE_WHAT_YOU_USE "Build with Include What You Use" OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Global Flags

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
  include(cmake/msvc-runtime.cmake)
endif()

# Compiler Warnings and Sanitizers
include(cmake/CompilerWarnings.cmake)
include(cmake/Sanitizers.cmake)

add_library(project_options INTERFACE)
add_library(project_warnings INTERFACE)

set_project_warnings(project_warnings)
enable_sanitizers(project_options)

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
