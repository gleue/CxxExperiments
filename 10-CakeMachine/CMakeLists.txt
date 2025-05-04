cmake_minimum_required (VERSION 3.10)
project (cake_machine)

add_executable(cake_machine cake_machine.cpp Shelf.cpp)

target_compile_features(cake_machine PUBLIC cxx_std_20)
