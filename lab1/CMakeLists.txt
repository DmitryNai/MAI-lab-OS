cmake_minimum_required(VERSION 3.10)

project(ProcessCommunication)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

execute_process(
    COMMAND git rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/../${GIT_BRANCH})
add_executable(parent parent.cpp)   
add_executable(child child.cpp)   
