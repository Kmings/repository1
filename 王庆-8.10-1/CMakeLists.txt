cmake_minimum_required(VERSION 3.10)

project(task8_10-1)

include_directories(${CMAKE_SOURCE_DIR}/inc)

add_library(TOOLS_STATIC STATIC tools/queue.c tools/stack.c)

add_executable(task8_10-1 ./src/main.c) 
target_link_libraries(task8_10-1 TOOLS_STATIC)
