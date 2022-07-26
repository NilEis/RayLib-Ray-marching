cmake_minimum_required(VERSION 3.0.0)
project(RayLibShader VERSION 0.1.0)

set(C_STANDARD 11)

add_subdirectory(extern/raylib)
add_subdirectory(src/audio)
add_subdirectory(src/video)

add_executable(RayLibShader src/main.c)

file(READ ${CMAKE_SOURCE_DIR}/shader/main.fs MAIN_SHADER_fs_file)
string(REPLACE "\n" "\\n\"\\\n\"" MAIN_SHADER_fs "${MAIN_SHADER_fs_file}")
file(READ ${CMAKE_SOURCE_DIR}/shader/main.vs MAIN_SHADER_vs_file)
string(REPLACE "\n" "\\n\"\\\n\"" MAIN_SHADER_vs "${MAIN_SHADER_vs_file}")

configure_file(${CMAKE_SOURCE_DIR}/include/video/shader.h.in ${CMAKE_SOURCE_DIR}/include/video/shader.h @ONLY)


target_link_libraries(${PROJECT_NAME} PUBLIC raylib audio video)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})