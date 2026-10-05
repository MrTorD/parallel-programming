cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0135 NEW)

set(CMAKE_CXX_STANDARD 23)

set(ROOT_PROJECT_NAME "parallel-programming")
project(${ROOT_PROJECT_NAME})

add_subdirectory(lw1)
add_subdirectory(lw2)
