add_executable(app app.cpp)
target_link_libraries(app mlcmst)

add_executable(ortools_ex ortools_example.cpp)
target_link_libraries(ortools_ex mlcmst)
