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
include_directories("${PROJECT_SOURCE_DIR}/src/imgui-docking")
include_directories("${PROJECT_SOURCE_DIR}/src/imgui-docking/backends")

add_executable(main 
    "${PROJECT_SOURCE_DIR}/src/glad/src/glad.c"
    "${PROJECT_SOURCE_DIR}/src/main.cpp"
    "${PROJECT_SOURCE_DIR}/src/stb_image.cpp"

    "${PROJECT_SOURCE_DIR}/src/imgui-docking/imgui.cpp"
    "${PROJECT_SOURCE_DIR}/src/imgui-docking/imgui_demo.cpp"
    "${PROJECT_SOURCE_DIR}/src/imgui-docking/imgui_draw.cpp"
    "${PROJECT_SOURCE_DIR}/src/imgui-docking/imgui_tables.cpp"
    "${PROJECT_SOURCE_DIR}/src/imgui-docking/imgui_widgets.cpp"
    "${PROJECT_SOURCE_DIR}/src/imgui-docking/backends/imgui_impl_glfw.cpp"
    "${PROJECT_SOURCE_DIR}/src/imgui-docking/backends/imgui_impl_opengl3.cpp"
)
if (MSVC)
    target_compile_options(main PRIVATE /W4)
else()
    message("Note: I only really tested it with MSVC since i use raddbg")
    target_compile_options(main PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_custom_command(
    TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/src/shaders"
    "$<TARGET_FILE_DIR:main>/shaders"
)
add_custom_command(
    TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:main>/assets"
)

# target_link_options(main PRIVATE -mwindows)
target_link_libraries(main PRIVATE glfw)
