cmake_minimum_required(VERSION 3.20.0)

project(github_little_try
        VERSION 0.0.1
        LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 11)
include_directories(include)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_LIST)

