
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
