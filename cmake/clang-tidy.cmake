find_program(clang-tidy clang-tidy)

if(NOT clang-tidy)
    message(STATUS "clang-tidy not found.")
else()
    message(STATUS "clang-tidy found: ${clang-tidy}")

    set_target_properties(
        ace
        board
        io
        search
        utils
        unittest
        perfttest
        PROPERTIES CXX_CLANG_TIDY "${clang-tidy}")
endif()