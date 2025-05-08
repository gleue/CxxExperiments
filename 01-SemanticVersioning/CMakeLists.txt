cmake_minimum_required(VERSION 4.0)
project(semver)

add_executable(semver main.cpp VersionRange.cpp SemanticVersion.cpp)

target_compile_features(semver PUBLIC cxx_std_20)