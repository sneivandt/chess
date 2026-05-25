cmake_minimum_required(VERSION 3.25)

if(POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW)
endif()

# Project

project(chess VERSION 1.0.0 LANGUAGES CXX)

# Options

option(CLANG_TIDY "Build with Clang-Tidy" OFF)
option(INCLUDE_WHAT_YOU_USE "Build with Include What You Use" OFF)
option(ACE_BUILD_TESTS "Build tests" ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Language standard

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

target_compile_features(project_options INTERFACE cxx_std_20)

set_project_warnings(project_warnings)
enable_sanitizers(project_options)

# Source

add_subdirectory(src)

# Test

if(ACE_BUILD_TESTS)
    enable_testing()

    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.17.0
        SYSTEM
        FIND_PACKAGE_ARGS NAMES GTest
    )
    set(INSTALL_GTEST OFF CACHE INTERNAL "Do not install gtest")
    set(BUILD_GMOCK OFF CACHE INTERNAL "Do not build gmock")
    FetchContent_MakeAvailable(googletest)

    add_subdirectory(test)
endif()

# Tools

set(ACE_TOOL_TARGETS ace board io search utils unittest perfttest)

include(cmake/clang-format.cmake)

if(CLANG_TIDY)
    include(cmake/clang-tidy.cmake)
endif()

if(INCLUDE_WHAT_YOU_USE)
    include(cmake/include-what-you-use.cmake)
endif()
