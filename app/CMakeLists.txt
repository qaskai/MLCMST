add_executable(generation generation.cpp)
target_link_libraries(generation cxxopts mlcmst)

add_executable(test_case_wrapper test_case_wrapper.cpp)
target_link_libraries(test_case_wrapper cxxopts mlcmst)

add_executable(benchmark benchmark/benchmark.cpp benchmark/solver_builder.cpp)
target_link_libraries(benchmark cxxopts mlcmst)


add_executable(playground playground.cpp)
target_compile_features(playground PUBLIC cxx_std_17)
target_link_libraries(playground mlcmst)
