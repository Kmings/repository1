cmake_minimum_required(VERSION 3.10)

project(task8_8_2)

include_directories(${CMAKE_SOURCE_DIR}/inc)

add_library(TOOLS_STATIC STATIC tools/vector_math.c)

add_executable(task8_8_2 src/solve.c)
target_link_libraries(task8_8_2 PRIVATE TOOLS_STATIC m)

# install(TARGETS task8_8_2 DESTINATION bin)
# install(FILES inc/main.h DESTINATION include)
# install(FILES inc/vector_math.h DESTINATION include)