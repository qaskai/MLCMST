FetchContent_Declare(
catch2
GIT_REPOSITORY "https://github.com/catchorg/Catch2.git"
GIT_TAG master
)
FetchContent_MakeAvailable(catch2)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${catch2_SOURCE_DIR}/contrib")

add_library(catch_main main.cpp)
target_link_libraries(catch_main PUBLIC Catch2::Catch2)

file(
    GLOB_RECURSE TESTS_SRC
    "*test.cpp"
)

add_executable(mlcmst_tests "${TESTS_SRC}")
target_link_libraries(mlcmst_tests catch_main mlcmst)

include(Catch)
catch_discover_tests(mlcmst_tests)

