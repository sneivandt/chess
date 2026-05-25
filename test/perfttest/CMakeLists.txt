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

add_custom_command(
    TARGET perfttest
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/perft.txt
            $<TARGET_FILE_DIR:perfttest>/perft.txt
    VERBATIM)

add_test(NAME perfttest
    COMMAND perfttest
    WORKING_DIRECTORY $<TARGET_FILE_DIR:perfttest>)
