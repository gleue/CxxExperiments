cmake_minimum_required(VERSION 4.0)
project(initialization)

add_executable(initialization initialization.cpp)

target_compile_features(initialization PUBLIC cxx_std_20)
