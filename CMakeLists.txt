cmake_minimum_required(VERSION 3.5)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 17)

project(main)

add_subdirectory("${PROJECT_SOURCE_DIR}/src/glfw-3.4")

include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories("${PROJECT_SOURCE_DIR}/src/glad/include")
include_directories("${PROJECT_SOURCE_DIR}/src/glad/src")

add_executable(main 
    "${PROJECT_SOURCE_DIR}/src/glad/src/glad.c"
    "${PROJECT_SOURCE_DIR}/src/main.cpp"
)
target_compile_options(main PRIVATE -Wall -Wextra -Wpedantic)

# target_link_options(main PRIVATE -mwindows)
target_link_libraries(main PRIVATE glfw)
