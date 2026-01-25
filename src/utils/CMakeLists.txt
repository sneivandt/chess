add_library(utils
    utils.cc
    include/utils/utils.h)

add_library(ace::utils ALIAS utils)

target_link_libraries(utils
    PRIVATE
        project_options
        project_warnings)

target_include_directories(utils
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR})