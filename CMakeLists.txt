cmake_minimum_required(VERSION 3.6)

# Project

project(chess VERSION 1.0.0)

# Options

option(CLANG_TIDY "Build with Clang-Tidy" OFF)
option(INCLUDE_WHAT_YOU_USE "Build with Include What You Use" OFF)

# Compiler features

set(CMAKE_CXX_STANDARD 17)

# MSVC static linking

if(MSVC)
    if("${MSVC_RUNTIME}" STREQUAL "")
        set(MSVC_RUNTIME "static")
    endif()
    set(variables
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_MINSIZEREL
        CMAKE_C_FLAGS_RELEASE
        CMAKE_C_FLAGS_RELWITHDEBINFO
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_MINSIZEREL
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_RELWITHDEBINFO
    )
    if(${MSVC_RUNTIME} STREQUAL "static")
        message(STATUS
            "MSVC -> forcing use of statically-linked runtime."
        )
        foreach(variable ${variables})
            if(${variable} MATCHES "/MD")
                string(REGEX REPLACE "/MD" "/MT" ${variable} "${${variable}}")
            endif()
        endforeach()
    else()
        message(STATUS
            "MSVC -> forcing use of dynamically-linked runtime."
        )
        foreach(variable ${variables})
            if(${variable} MATCHES "/MT")
                string(REGEX REPLACE "/MT" "/MD" ${variable} "${${variable}}")
            endif()
        endforeach()
    endif()
endif()

# Source

add_subdirectory(src)

# Test

enable_testing()

option(INSTALL_GTEST OFF)
option(BUILD_GMOCK OFF)
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