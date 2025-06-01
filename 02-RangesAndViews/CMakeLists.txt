add_executable(ranges_and_views ranges_and_views.cpp)

target_compile_features(ranges_and_views PUBLIC cxx_std_23)

add_executable(cities main.cpp City.cpp)

target_compile_features(cities PUBLIC cxx_std_23)
