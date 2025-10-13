cmake_minimum_required(VERSION 3.14)
project(threaded_tree LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(flags)

add_executable(tree
    source/main.cpp
)

target_include_directories(tree PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(tree PRIVATE flags)