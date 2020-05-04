add_executable(app app.cpp)
target_link_libraries(app mlcmst)

add_executable(generation generation.cpp)
target_link_libraries(generation cxxopts mlcmst)