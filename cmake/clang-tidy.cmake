find_program(CLANG_TIDY_EXE clang-tidy)

if(NOT CLANG_TIDY_EXE)
    message(STATUS "clang-tidy not found.")
else()
    message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")

    foreach(target IN LISTS ACE_TOOL_TARGETS)
        if(TARGET ${target})
            set_target_properties(${target} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
        endif()
    endforeach()
endif()
