cmake_minimum_required(VERSION 3.6)
project(chess VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 11)

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

if(NOT CI)
    include(cmake/clang-format.cmake)
    include(cmake/clang-tidy.cmake)
    include(cmake/include-what-you-use.cmake)
endif()