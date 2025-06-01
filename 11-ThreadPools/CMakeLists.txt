add_executable(threadpools main.cpp ThreadPool.cpp)

target_compile_features(threadpools PUBLIC cxx_std_20)
target_link_libraries(threadpools PRIVATE Qt6::Core Qt6::Widgets)
