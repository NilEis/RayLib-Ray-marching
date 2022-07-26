cmake_minimum_required(VERSION 3.0.0)
project(audio)


add_library(${PROJECT_NAME} audio_main.c)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include/ ${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})