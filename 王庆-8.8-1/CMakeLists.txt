cmake_minimum_required(VERSION 3.10)

project(task8_8)

include_directories(${CMAKE_SOURCE_DIR}/inc)

add_library(TOOLS_STATIC STATIC tools/method1.c tools/method2.c tools/method3.c)

add_executable(task8_8 ./src/main.c) 
target_link_libraries(task8_8 TOOLS_STATIC)