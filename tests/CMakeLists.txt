FetchContent_Declare(
catch2
GIT_REPOSITORY https://github.com/catchorg/Catch2.git
)
FetchContent_MakeAvailable(catch2)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${catch2_SOURCE_DIR}/contrib")

add_library(catch_main main.cpp)
target_link_libraries(catch_main PUBLIC Catch2::Catch2)

file(
    GLOB_RECURSE TESTS_SRC
    "*.cpp"
)
list(REMOVE_ITEM TESTS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

add_executable(MLCMST_tests "${TESTS_SRC}")
target_link_libraries(MLCMST_tests catch_main)

include(Catch)
catch_discover_tests(MLCMST_tests)

