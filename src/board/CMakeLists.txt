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

target_include_directories(board
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR})