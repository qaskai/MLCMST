add_executable(generation generation.cpp)
target_link_libraries(generation cxxopts mlcmst)

add_executable(test_case_wrapper test_case_wrapper.cpp)
target_link_libraries(test_case_wrapper cxxopts mlcmst)
