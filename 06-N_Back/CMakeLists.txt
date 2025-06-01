add_executable(n_back main.cpp NBackTest.cpp NBackSequence.cpp)

target_compile_features(n_back PUBLIC cxx_std_20)
