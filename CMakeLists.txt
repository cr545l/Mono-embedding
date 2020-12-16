cmake_minimum_required(VERSION 3.0.0)

project(mono_embedding VERSION 1.0.0)

set(PROJECT_ROOT ${mono_embedding})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_subdirectory(cpp)
add_subdirectory(csharp)
