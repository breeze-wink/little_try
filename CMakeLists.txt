cmake_minimum_required(VERSION 3.20.0)
project(epoll
    VERSION 0.0.1
    DESCRIPTION "description of epoll"
    HOMEPAGE_URL "https://github.com/breeze-wink/little_try"
    LANGUAGES CXX
)   

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

set(EventPoller_Dir ${CMAKE_SOURCE_DIR}/src/EventPoller)
set(Socket_Dir ${CMAKE_SOURCE_DIR}/src/Socket)
set(SocketHandler_Dir ${CMAKE_SOURCE_DIR}/src/SocketHandler)
set(Task_Dir ${CMAKE_SOURCE_DIR}/src/Task)
set(Utility_Dir ${CMAKE_SOURCE_DIR}/src/utility)


add_subdirectory(src)