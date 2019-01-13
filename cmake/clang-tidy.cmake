find_program(clang-tidy clang-tidy)

if(NOT clang-tidy)
    message(STATUS "clang-tidy not found.")
else()
    message(STATUS "clang-tidy found: ${clang-tidy}")

    set(DO_CLANG_TIDY "${clang-tidy}"
        "-checks= \
            -*, \
            bugprone-*, \
            cert-*, \
            clang-analyzer-*, \
            cppcoreguidelines-*, \
            -cppcoreguidelines-pro-bounds-constant-array-index, \
            -cppcoreguidelines-pro-bounds-pointer-arithmetic, \
            google-*, \
            llvm-*, \
            -llvm-include-order, \
            misc-*, \
            modernize-*, \
            performance-*, \
            portability-*, \
            readability-*, \
            -readability-implicit-bool-conversion")

    set_target_properties(
        ace
        board
        io
        search
        utils
        unittest
        perfttest
        PROPERTIES CXX_CLANG_TIDY "${DO_CLANG_TIDY}")
endif()