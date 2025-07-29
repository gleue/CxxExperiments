include(FetchContent)
 
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
  GIT_TAG v6.1.9  # Replace with a version, tag, or commit hash
)
 
FetchContent_MakeAvailable(ftxui)
 
add_executable(clock clock.cpp)

target_compile_features(clock PUBLIC cxx_std_20)

target_link_libraries(clock PRIVATE ftxui::component ftxui::dom ftxui::screen)
