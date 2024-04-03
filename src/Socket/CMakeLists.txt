project (Socket)

aux_source_directory(. SRC_LIST)

add_library(${PROJECT_NAME} SHARED ${SRC_LIST})


target_include_directories(${PROJECT_NAME} PUBLIC ${Utility_Dir})
target_link_libraries(${PROJECT_NAME} PUBLIC Utility)