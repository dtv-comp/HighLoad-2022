cmake_minimum_required(VERSION 3.5)

project(shmem_example LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(shmem_example main.cpp Shmem.cpp Tables.cpp ShmemProc.cpp MainProc.cpp ReaderProc.cpp)
target_link_libraries(shmem_example rt pthread)

