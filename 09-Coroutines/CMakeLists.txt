add_executable(card_dealer card_dealer.cpp)

target_compile_features(card_dealer PUBLIC cxx_std_20)
target_compile_options(card_dealer PUBLIC -fcoroutines)
