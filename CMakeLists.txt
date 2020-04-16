cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(
MLCMST
VERSION 0.1.0
LANGUAGES CXX
)


if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif()

include(FetchContent)

FetchContent_Declare(
or-tools
GIT_REPOSITORY "https://github.com/google/or-tools.git"
GIT_TAG master
)
FetchContent_MakeAvailable(or-tools)


include_directories(include)

add_subdirectory(src)
add_subdirectory(app)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
