cmake_minimum_required(VERSION 4.0)
project(move_semantics)

add_executable(move_semantics move_semantics.cpp)

target_compile_features(move_semantics PUBLIC cxx_std_20)

add_executable(messages main.cpp Message.cpp)

target_compile_features(messages PUBLIC cxx_std_20)
