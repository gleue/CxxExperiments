cmake_minimum_required (VERSION 3.5)
project (ranges_and_views)

add_executable(ranges_and_views main.cpp)

target_compile_features(ranges_and_views PUBLIC cxx_std_20)
