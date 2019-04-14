add_library(search
    movegen.cc
    movelist.cc
    pvtable.cc
    pvtable.h
    search.cc
    searchinfo.cc
    include/search/movegen.h
    include/search/movelist.h
    include/search/search.h
    include/search/searchinfo.h)

add_library(ace::search ALIAS search)

if(MSVC)
    target_compile_options(search
        PRIVATE
            /W3
            /WX
            $<$<CONFIG:DEBUG>:/Odi>
            $<$<CONFIG:RELEASE>:/O2>)
else()
    add_compile_options(search
        PRIVATE
            -Wall
            -Wextra
            -Werror
            -pedantic
            $<$<CONFIG:DEBUG>:-O0>
            $<$<CONFIG:RELEASE>:-O3>)
endif()

target_include_directories(search
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(search
    PRIVATE
        ace::board
        ace::utils)