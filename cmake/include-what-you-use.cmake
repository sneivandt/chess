find_program(INCLUDE_WHAT_YOU_USE_EXE include-what-you-use)

if(NOT INCLUDE_WHAT_YOU_USE_EXE)
    message(STATUS "include-what-you-use not found.")
else()
    message(STATUS "include-what-you-use found: ${INCLUDE_WHAT_YOU_USE_EXE}")

    foreach(target IN LISTS ACE_TOOL_TARGETS)
        if(TARGET ${target})
            set_target_properties(${target} PROPERTIES CXX_INCLUDE_WHAT_YOU_USE "${INCLUDE_WHAT_YOU_USE_EXE}")
        endif()
    endforeach()
endif()
