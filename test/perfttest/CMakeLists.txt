add_executable(perfttest
    perfttest.cc
    include/perfttest/perfttest.h)

target_include_directories(perfttest
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

target_link_libraries(perfttest
    PRIVATE 
        gtest
        gtest_main
        ace::board
        ace::search
        ace::utils)

file(COPY perft.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME perfttest
    COMMAND perfttest)