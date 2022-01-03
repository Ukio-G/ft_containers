cmake_minimum_required(VERSION 3.20)
project(ft_containers)

include_directories(ft_vector ft_map ft_stack ft_meta_cpp .)

set(CMAKE_CXX_STANDARD 98)

add_executable(ft_containers main.cpp ft_map/map.cpp ft_stack/stack.cpp ft_vector/vector.cpp)
