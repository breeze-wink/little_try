project(epoll_app)

include_directories(${Utility_Dir} ${Task_Dir} ${Socket_Dir} ${SocketHandler_Dir} ${EventPoller_Dir})

add_executable(epoll_server server2.cpp)
target_link_libraries(epoll_server PUBLIC Task SocketHandler Socket EventPoller)
add_executable(client client.cpp)
target_link_libraries(client PUBLIC Utility Socket)