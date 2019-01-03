add_executable(perft
    perft.cc
    include/perft/perft.h)

target_include_directories(perft
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

target_link_libraries(perft
    PRIVATE ace::board
    PRIVATE ace::search
    PRIVATE ace::utils)

file(COPY perft.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME perft
    COMMAND perft)