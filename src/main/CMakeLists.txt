add_executable(ace
    main.cc)

target_link_libraries(ace
    PRIVATE ace::board
    PRIVATE ace::io
    PRIVATE ace::search
    PRIVATE ace::utils)

install(TARGETS ace EXPORT ace-targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION includes)

install(EXPORT ace-targets
    DESTINATION lib/cmake/ace
    FILE ace-targets.cmake
    NAMESPACE ace::)