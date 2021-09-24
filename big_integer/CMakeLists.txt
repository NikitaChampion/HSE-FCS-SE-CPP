cmake_minimum_required(VERSION 3.19)
project(big_integer)

set(CMAKE_CXX_STANDARD 17)

add_executable(big_integer main.cpp big_integer_lib/big_int.h big_integer_lib/big_int.cpp)