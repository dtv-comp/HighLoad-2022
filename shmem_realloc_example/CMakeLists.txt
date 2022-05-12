cmake_minimum_required(VERSION 3.5)

project(shmem_realloc_example LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(shmem_realloc_example main.cpp)
target_link_libraries(shmem_realloc_example rt pthread)

