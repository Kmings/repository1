cmake_minimum_required(VERSION 3.10)

project(task8_9-1)

include_directories(${CMAKE_SOURCE_DIR}/inc)

add_library(TOOLS_STATIC STATIC tools/tools.c)

add_executable(task8_9-1 ./src/main.c) 
target_link_libraries(task8_9-1 TOOLS_STATIC)
