cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(
MLCMST-experiments
VERSION 0.1.0
LANGUAGES CXX
)


if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif()


include(FetchContent)

include_directories(include)

add_subdirectory(src)
add_subdirectory(app)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
