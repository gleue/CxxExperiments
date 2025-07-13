add_executable(pool pool.cpp ThreadPool.cpp)

target_compile_features(pool PUBLIC cxx_std_23)

add_executable(threadpools main.cpp BackgroundTask.cpp Progress.cpp ProgressView.cpp ThreadPool.cpp)
target_sources(threadpools PRIVATE BackgroundTask.h Progress.h ProgressView.h ThreadPool.h)

set_target_properties(threadpools PROPERTIES AUTOMOC ON)

target_compile_features(threadpools PUBLIC cxx_std_20)
target_link_libraries(threadpools PRIVATE Qt6::Core Qt6::Widgets)
