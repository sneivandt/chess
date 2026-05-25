find_program(CLANG_FORMAT_EXE clang-format)

if(NOT CLANG_FORMAT_EXE)
    message(STATUS "clang-format not found")
else()
    message(STATUS "clang-format found: ${CLANG_FORMAT_EXE}")

    file(GLOB_RECURSE ALL_SOURCE_FILES CONFIGURE_DEPENDS
        ${PROJECT_SOURCE_DIR}/src/*.cc
        ${PROJECT_SOURCE_DIR}/src/*.h
        ${PROJECT_SOURCE_DIR}/test/*.cc
        ${PROJECT_SOURCE_DIR}/test/*.h)

    add_custom_target(
        clang-format
        COMMAND ${CLANG_FORMAT_EXE}
        -style=file
        -i
        ${ALL_SOURCE_FILES}
        VERBATIM)
endif()
