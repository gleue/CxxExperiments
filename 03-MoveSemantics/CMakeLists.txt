cmake_minimum_required (VERSION 3.5)
project (move_semantics)

add_executable(move_semantics move_semantics.cpp Message.cpp)

target_compile_features(move_semantics PUBLIC cxx_std_20)
