cmake_minimum_required(VERSION 3.22)

project (shared_ptr VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)

set(MY_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")

add_subdirectory(src)