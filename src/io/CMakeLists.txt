add_library(io
    io.cc
    uci.cc
    include/io/io.h
    include/io/uci.h)

add_library(ace::io ALIAS io)

if(MSVC)
    target_compile_options(io
        PRIVATE
            /W3
            /WX
            $<$<CONFIG:DEBUG>:/Odi>
            $<$<CONFIG:RELEASE>:/O2>)
else()
    add_compile_options(io
        PRIVATE
            -Wall
            -Wextra
            -Werror
            -pedantic
            $<$<CONFIG:DEBUG>:-O0>
            $<$<CONFIG:RELEASE>:-O3>)
endif()

target_include_directories(io
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(io
    PRIVATE 
        ace::board
        ace::search
        ace::utils)