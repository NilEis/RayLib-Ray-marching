cmake_minimum_required(VERSION 3.0.0)
project(video)


add_library(${PROJECT_NAME} video_main.c)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include/ ${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
target_link_libraries(${PROJECT_NAME} raylib)