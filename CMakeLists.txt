cmake_minimum_required(VERSION 3.0.0)
project(Ray-marching VERSION 0.1.0)

set(C_STANDARD 11)

if (EMSCRIPTEN)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY")
  set(CMAKE_EXECUTABLE_SUFFIX ".html") # This line is used to set your executable to build with the emscripten html template so taht you can directly open it.
endif ()

add_subdirectory(extern/raylib)
add_subdirectory(extern/raygui/projects/CMake)
add_subdirectory(src/audio)
add_subdirectory(src/video)

add_executable(Ray-marching src/main.c)

set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/shader/main.frag)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/shader/main.vert)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/shader/pp_1.frag)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/shader/blur.frag)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/shader/hdr.frag)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/shader/gamma.frag)
file(READ ${CMAKE_SOURCE_DIR}/shader/main.frag MAIN_SHADER_fs_file)
string(REPLACE "\n" "\\n\"\\\n\"" MAIN_SHADER_fs "${MAIN_SHADER_fs_file}")
file(READ ${CMAKE_SOURCE_DIR}/shader/main.vert MAIN_SHADER_vs_file)
string(REPLACE "\n" "\\n\"\\\n\"" MAIN_SHADER_vs "${MAIN_SHADER_vs_file}")
file(READ ${CMAKE_SOURCE_DIR}/shader/pp_1.frag PP_1_SHADER_fs_file)
string(REPLACE "\n" "\\n\"\\\n\"" PP_SHADER_1_fs "${PP_1_SHADER_fs_file}")
file(READ ${CMAKE_SOURCE_DIR}/shader/blur.frag BLUR_SHADER_fs_file)
string(REPLACE "\n" "\\n\"\\\n\"" BLUR_SHADER_fs "${BLUR_SHADER_fs_file}")
file(READ ${CMAKE_SOURCE_DIR}/shader/hdr.frag HDR_SHADER_fs_file)
string(REPLACE "\n" "\\n\"\\\n\"" HDR_SHADER_fs "${HDR_SHADER_fs_file}")
file(READ ${CMAKE_SOURCE_DIR}/shader/gamma.frag GAMMA_SHADER_fs_file)
string(REPLACE "\n" "\\n\"\\\n\"" GAMMA_SHADER_fs "${GAMMA_SHADER_fs_file}")

configure_file(${CMAKE_SOURCE_DIR}/include/video/shader.h.in ${CMAKE_SOURCE_DIR}/include/video/shader.h @ONLY)

target_link_libraries(${PROJECT_NAME} PUBLIC raylib raygui audio video)