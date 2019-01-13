find_program(include-what-you-use include-what-you-use)

if(NOT include-what-you-use)
    message(STATUS "include-what-you-use not found.")
else()
    message(STATUS "include-what-you-use found: ${include-what-you-use}")

    set_target_properties(
        ace
        board
        io
        search
        utils
        unittest
        perfttest
        PROPERTIES CXX_INCLUDE_WHAT_YOU_USE ${include-what-you-use})
endif()