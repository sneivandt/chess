find_program(clang-format clang-format)

if(NOT clang-format)
    message(STATUS "clang-format not found")
else()
    message(STATUS "clang-format found: ${clang-format}")

    file(GLOB_RECURSE ALL_SOURCE_FILES
        src/**/*.cc
        src/**/*.h
        test/**/*.cc
        test/**/*.h)

    add_custom_target(
        clang-format ALL
        COMMAND ${clang-format}
        -style=file
        -i
        ${ALL_SOURCE_FILES}
    )
endif()
