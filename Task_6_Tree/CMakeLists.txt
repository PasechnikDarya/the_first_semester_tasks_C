cmake_minimum_required(VERSION 3.12)
project(C)

set(CMAKE_C_STANDARD 99)

add_executable(main.c tree_head.h tree.c)