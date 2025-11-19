cmake_minimum_required(VERSION 3.14)
project(threaded_tree LANGUAGES CXX)

add_subdirectory(flags)

add_executable(tree
    source/main.cpp
)

target_include_directories(tree PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(tree PRIVATE flags)
target_compile_definitions(tree PRIVATE USE_MY_TREE)

add_executable(std_tree
    source/main.cpp
)

target_include_directories(std_tree PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(std_tree PRIVATE flags)
target_compile_definitions(std_tree PRIVATE USE_STD_TREE)

add_executable(bench_tree
    source/bench_main.cpp
)

target_include_directories(bench_tree PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(bench_tree PRIVATE flags)
target_compile_definitions(bench_tree PRIVATE USE_MY_TREE)

add_executable(bench_std_tree
    source/bench_main.cpp
)

target_include_directories(bench_std_tree PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(bench_std_tree PRIVATE flags)
target_compile_definitions(bench_std_tree PRIVATE USE_STD_TREE)