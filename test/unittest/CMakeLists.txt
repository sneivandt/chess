add_executable(unittest
    bitboardtest.cc
    boardtest.cc
    boardtest.h
    iotest.cc
    iotest.h
    makemovetest.cc
    movelisttest.cc
    movegentest.cc
    movetest.cc
    evaluatetest.cc
    searchtest.cc
    ucitest.cc
    ucitest.h
    unittest.h)

target_link_libraries(unittest
    PRIVATE
        project_options
        project_warnings
        GTest::gtest
        GTest::gtest_main
        ace::board
        ace::search
        ace::io
        ace::utils)

target_include_directories(unittest
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR})

add_test(NAME unittest
    COMMAND unittest)
