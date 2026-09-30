cmake_minimum_required(VERSION 3.10.0)
project(Prog_THU VERSION 0.1.0 LANGUAGES C CXX)

add_executable(Prog_THU number4.cpp)

target_compile_options(Prog_THU PRIVATE -O3 -march=native)