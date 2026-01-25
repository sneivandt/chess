add_executable(perfttest
    perfttest.cc
    perfttest.h)

target_link_libraries(perfttest
    PRIVATE
        project_options
        project_warnings)

target_include_directories(perfttest
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR})

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