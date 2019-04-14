add_executable(ace
    main.cc)

if(MSVC)
    target_compile_options(ace
        PRIVATE
            /W3
            /WX
            $<$<CONFIG:DEBUG>:/Odi>
            $<$<CONFIG:RELEASE>:/O2>)
else()
    add_compile_options(ace
        PRIVATE
            -Wall
            -Wextra
            -Werror
            -pedantic
            $<$<CONFIG:DEBUG>:-O0>
            $<$<CONFIG:RELEASE>:-O3>)
endif()

target_link_libraries(ace
    PRIVATE
        ace::board
        ace::io
        ace::search
        ace::utils)

install(TARGETS ace EXPORT ace-targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION includes)