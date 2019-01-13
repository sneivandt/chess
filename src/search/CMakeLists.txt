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

target_include_directories(search
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(search
    PRIVATE 
        ace::board
        ace::utils)