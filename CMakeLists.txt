cmake_minimum_required(VERSION 3.20)
project(ft_containers)

include_directories(ft_vector ft_map ft_stack ft_meta_cpp .)

set(CMAKE_CXX_STANDARD 98)
add_definitions("-O0 -g3")

add_executable(ft_containers main.cpp)
