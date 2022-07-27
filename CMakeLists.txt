cmake_minimum_required(VERSION 3.0.0)
project(Ray-marching VERSION 0.1.0)

set(C_STANDARD 11)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(extern/raylib)
set(BUILD_RAYGUI_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(extern/raygui/projects/CMake)
add_subdirectory(src/audio)
add_subdirectory(src/video)

add_executable(Ray-marching src/main.c)

set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/shader/main.fs)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/shader/main.vs)
file(READ ${CMAKE_SOURCE_DIR}/shader/main.fs MAIN_SHADER_fs_file)
string(REPLACE "\n" "\\n\"\\\n\"" MAIN_SHADER_fs "${MAIN_SHADER_fs_file}")
file(READ ${CMAKE_SOURCE_DIR}/shader/main.vs MAIN_SHADER_vs_file)
string(REPLACE "\n" "\\n\"\\\n\"" MAIN_SHADER_vs "${MAIN_SHADER_vs_file}")

configure_file(${CMAKE_SOURCE_DIR}/include/video/shader.h.in ${CMAKE_SOURCE_DIR}/include/video/shader.h @ONLY)

target_link_libraries(${PROJECT_NAME} PUBLIC raylib raygui audio video)