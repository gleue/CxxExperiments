cmake_minimum_required(VERSION 4.0)
project(constness)

add_executable(constness main.cpp)

target_compile_features(n_back PUBLIC cxx_std_20)
