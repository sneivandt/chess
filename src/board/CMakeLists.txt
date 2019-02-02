add_library(board
    bitboard.cc
    board.cc
    evaluate.cc
    makemove.cc
    move.cc
    bitboard.h
    include/board/board.h
    include/board/evaluate.h
    include/board/makemove.h
    include/board/move.h
    include/board/undo.h)

add_library(ace::board ALIAS board)

if(MSVC)
    target_compile_options(board
        PRIVATE
            /W3
            /WX
            $<$<CONFIG:DEBUG>:/Odi>
            $<$<CONFIG:RELEASE>:/O2>)
else()
    add_compile_options(board
        PRIVATE
            -Wall
            -Wextra
            -Werror
            -pedantic
            $<$<CONFIG:DEBUG>:-O0>
            $<$<CONFIG:RELEASE>:-O3>)
endif()

target_include_directories(board
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR})