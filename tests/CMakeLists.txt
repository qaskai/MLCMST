FetchContent_Declare(
catch2
GIT_REPOSITORY https://github.com/catchorg/Catch2.git
)
FetchContent_MakeAvailable(catch2)

add_executable(dummy_test main.cpp test.cpp)
target_link_libraries(dummy_test Catch2::Catch2)

add_test(NAME dummy_test COMMAND dummy_test)
