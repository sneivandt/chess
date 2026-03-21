add_executable(ace
    main.cc)

target_link_libraries(ace
    PRIVATE
        project_options
        project_warnings
        ace::board
        ace::io
        ace::search
        ace::utils)

install(TARGETS ace EXPORT ace-targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION includes)