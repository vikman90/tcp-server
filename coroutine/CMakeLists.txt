set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES main.cpp server.cpp)

if(APPLE)
    list(APPEND SOURCES poll_bsd.cpp)
elseif(LINUX)
    list(APPEND SOURCES poll_linux.cpp)
endif()

add_executable(server-cr ${SOURCES})

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(server-cr PRIVATE -fcoroutines)
endif()
