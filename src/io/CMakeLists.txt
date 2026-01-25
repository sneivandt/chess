add_library(io
    io.cc
    uci.cc
    include/io/io.h
    include/io/uci.h)

add_library(ace::io ALIAS io)

target_link_libraries(io
    PRIVATE
        project_options
        project_warnings)

target_include_directories(io
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(io
    PRIVATE
        ace::board
        ace::search
        ace::utils)