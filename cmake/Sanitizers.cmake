# from: https://github.com/lefticus/cpp_starter_project/blob/master/cmake/Sanitizers.cmake

function(enable_sanitizers project_name)

  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    option(ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
    option(ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(ENABLE_SANITIZER_UNDEFINED "Enable undefined behavior sanitizer" OFF)
    option(ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)

    set(SANITIZERS "")

    if(ENABLE_SANITIZER_ADDRESS)
      list(APPEND SANITIZERS "address")
    endif()

    if(ENABLE_SANITIZER_LEAK)
      list(APPEND SANITIZERS "leak")
    endif()

    if(ENABLE_SANITIZER_UNDEFINED)
      list(APPEND SANITIZERS "undefined")
    endif()

    if(ENABLE_SANITIZER_THREAD)
      if("address" IN_LIST SANITIZERS OR "leak" IN_LIST SANITIZERS)
        message(WARNING "Thread sanitizer does not work with Address or Leak sanitizer enabled")
      else()
        list(APPEND SANITIZERS "thread")
      endif()
    endif()

    if(ENABLE_SANITIZER_MEMORY)
      if("address" IN_LIST SANITIZERS
         OR "leak" IN_LIST SANITIZERS
         OR "thread" IN_LIST SANITIZERS)
        message(WARNING "Memory sanitizer does not work with Address, Leak, or Thread sanitizer enabled")
      else()
        list(APPEND SANITIZERS "memory")
      endif()
    endif()

    list(JOIN SANITIZERS "," LIST_OF_SANITIZERS)

    if(LIST_OF_SANITIZERS)
      if(NOT
         "${LIST_OF_SANITIZERS}"
         STREQUAL
         "")
        target_compile_options(${project_name} INTERFACE -fsanitize=${LIST_OF_SANITIZERS})
        target_link_options(${project_name} INTERFACE -fsanitize=${LIST_OF_SANITIZERS})
      endif()
    endif()
  endif()

endfunction()
