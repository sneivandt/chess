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

add_subdirectory(src/utils)
add_subdirectory(src/board)
add_subdirectory(src/search)
add_subdirectory(src/io)
add_subdirectory(src/main)

enable_testing()
add_subdirectory(extern/googletest EXCLUDE_FROM_ALL)
add_subdirectory(test EXCLUDE_FROM_ALL)

include(cmake/clang-format.cmake)
include(cmake/clang-tidy.cmake)
include(cmake/include-what-you-use.cmake)