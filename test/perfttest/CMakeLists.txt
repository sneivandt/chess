add_executable(perfttest
    perfttest.cc
    perfttest.h)

target_link_libraries(perfttest
    PRIVATE
        project_options
        project_warnings
        GTest::gtest
        GTest::gtest_main
        ace::board
        ace::search
        ace::utils)

target_include_directories(perfttest
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR})

file(COPY perft.txt
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME perfttest
    COMMAND perfttest)