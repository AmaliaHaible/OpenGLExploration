cmake_minimum_required(VERSION 3.15)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 17)

project(main)

add_subdirectory("${PROJECT_SOURCE_DIR}/src/glfw-3.4")

include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories("${PROJECT_SOURCE_DIR}/src/glad/include")
include_directories("${PROJECT_SOURCE_DIR}/src/glad/src")
# include_directories("${PROJECT_SOURCE_DIR}/src/glm-0.9.8.5")
include_directories("${PROJECT_SOURCE_DIR}/src/glm-1.0.1")

add_executable(main 
    "${PROJECT_SOURCE_DIR}/src/glad/src/glad.c"
    "${PROJECT_SOURCE_DIR}/src/main.cpp"
    "${PROJECT_SOURCE_DIR}/src/stb_image.cpp"
)
if (MSVC)
    target_compile_options(main PRIVATE /W4)
else()
    message("Note: I only really tested it with MSVC since i use raddbg")
    target_compile_options(main PRIVATE -Wall -Wextra -Wpedantic)
endif()
#
# configure_file(
#     "${PROJECT_SOURCE_DIR}/src/vertex.glsl"
#     "vertex.glsl"
#     COPYONLY
# )
# configure_file(
#     "${PROJECT_SOURCE_DIR}/src/fragment.glsl"
#     "fragment.glsl"
#     COPYONLY
# )
add_custom_command(
    TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${PROJECT_SOURCE_DIR}/src/vertex.glsl"
            "$<TARGET_FILE_DIR:main>/vertex.glsl"
)
add_custom_command(
    TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${PROJECT_SOURCE_DIR}/src/fragment.glsl"
            "$<TARGET_FILE_DIR:main>/fragment.glsl"
)
add_custom_command(
    TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${PROJECT_SOURCE_DIR}/assets/container.jpg"
            "$<TARGET_FILE_DIR:main>/container.jpg"
)
add_custom_command(
    TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${PROJECT_SOURCE_DIR}/assets/awesomeface.png"
            "$<TARGET_FILE_DIR:main>/awesomeface.png"
)

# target_link_options(main PRIVATE -mwindows)
target_link_libraries(main PRIVATE glfw)
