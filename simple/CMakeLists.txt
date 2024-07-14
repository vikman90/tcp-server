set(SOURCES buffer.c main.c server.c)

if(APPLE)
    list(APPEND SOURCES poll_bsd.c)
elseif(LINUX)
    list(APPEND SOURCES poll_linux.c)
endif()

add_executable(server-simple ${SOURCES})
