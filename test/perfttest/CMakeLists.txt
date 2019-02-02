add_executable(perfttest
    perfttest.cc
    perfttest.h)

if(MSVC)
    target_compile_options(perfttest
        PRIVATE
            /W3
            /WX
            $<$<CONFIG:DEBUG>:/Odi>
            $<$<CONFIG:RELEASE>:/O2>)
else()
    add_compile_options(perfttest
        PRIVATE
            -Wall
            -Wextra
            -Werror
            -pedantic
            $<$<CONFIG:DEBUG>:-O0>
            $<$<CONFIG:RELEASE>:-O3>)
endif()

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