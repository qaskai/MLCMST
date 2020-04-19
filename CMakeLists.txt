cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(
MLCMST
VERSION 0.1.0
LANGUAGES CXX
)

include("${CMAKE_CURRENT_SOURCE_DIR}/DownloadDependencies.cmake")

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif()

include_directories(include)

add_subdirectory(src)
add_subdirectory(app)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
