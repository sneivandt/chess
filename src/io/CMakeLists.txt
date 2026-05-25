add_library(io
    io.cc
    uci.cc
    include/io/io.h
    include/io/uci.h)

add_library(ace::io ALIAS io)

target_link_libraries(io
    PUBLIC
        ace::board
    PRIVATE
        project_options
        project_warnings
        ace::search
        ace::utils)

target_include_directories(io
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR})
