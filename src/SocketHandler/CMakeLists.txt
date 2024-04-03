project(SocketHandler)

add_library(${PROJECT_NAME} SHARED socket_handler.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${Socket_Dir})
target_include_directories(${PROJECT_NAME} PUBLIC ${Utility_Dir})
target_include_directories(${PROJECT_NAME} PUBLIC ${EventPoller_Dir})
target_include_directories(${PROJECT_NAME} PUBLIC ${Task_Dir})

target_link_libraries(${PROJECT_NAME} PUBLIC Socket Utility Task EventPoller)