project(Task)

add_library(${PROJECT_NAME} STATIC echo_task.cpp)


target_include_directories(${PROJECT_NAME} PUBLIC ${Socket_Dir})
target_include_directories(${PROJECT_NAME} PUBLIC ${Utility_Dir})

