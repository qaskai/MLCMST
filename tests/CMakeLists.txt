FetchContent_Declare(
catch2
GIT_REPOSITORY https://github.com/catchorg/Catch2.git
)
FetchContent_MakeAvailable(catch2)


add_library(catch_main main.cpp)
target_link_libraries(catch_main PUBLIC Catch2::Catch2)


add_executable(dummy_test test.cpp)
target_link_libraries(dummy_test catch_main)
add_test(NAME dummy_test COMMAND dummy_test)
