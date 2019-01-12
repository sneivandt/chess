add_executable(unittest
    boardtest.cc
    ucitest.cc
    include/unittest/boardtest.h
    include/unittest/ucitest.h
    include/unittest/unittest.h)

target_include_directories(unittest
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

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