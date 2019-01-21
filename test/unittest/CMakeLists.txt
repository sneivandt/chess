add_executable(unittest
    boardtest.cc
    boardtest.h
    ucitest.cc
    ucitest.h
    unittest.h)

target_include_directories(unittest
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(unittest
    PRIVATE
        gtest 
        gtest_main
        ace::board
        ace::search
        ace::io
        ace::utils)

add_test(NAME unittest
    COMMAND unittest)