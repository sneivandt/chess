cmake_minimum_required(VERSION 3.8)
project(chess VERSION 1.0.0)

if(MSVC)
    add_compile_options(
        /W3
        /WX)
else()
    add_compile_options(
        -Wall
        -Wextra
        -Werror
        -pedantic
        $<$<CONFIG:DEBUG>:-O0>
        $<$<CONFIG:RELEASE>:-O3>)
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(src/utils)
add_subdirectory(src/board)
add_subdirectory(src/search)
add_subdirectory(src/io)
add_subdirectory(src/main)

include(cmake/clang-format.cmake)
include(cmake/clang-tidy.cmake)

enable_testing()
add_subdirectory(test EXCLUDE_FROM_ALL)