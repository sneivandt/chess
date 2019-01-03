add_library(io
    io.cc
    uci.cc
    include/io/io.h
    include/io/uci.h)

add_library(ace::io ALIAS io)

target_include_directories(io
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

target_link_libraries(io
    PRIVATE ace::board
    PRIVATE ace::search
    PRIVATE ace::utils)