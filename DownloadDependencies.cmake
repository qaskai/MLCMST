include(FetchContent)

FetchContent_Declare(
        or-tools
        GIT_REPOSITORY "https://github.com/google/or-tools.git"
        GIT_TAG stable
)
FetchContent_MakeAvailable(or-tools)

FetchContent_Declare(
        glog
        GIT_REPOSITORY "https://github.com/google/glog.git"
        GIT_TAG master
)
FetchContent_MakeAvailable(glog)

FetchContent_Declare(
        cxxopts
        GIT_REPOSITORY "https://github.com/jarro2783/cxxopts.git"
        GIT_TAG master
)
FetchContent_MakeAvailable(cxxopts)

FetchContent_Declare(
        catch2
        GIT_REPOSITORY "https://github.com/catchorg/Catch2.git"
        GIT_TAG master
)
FetchContent_MakeAvailable(catch2)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${catch2_SOURCE_DIR}/contrib")

FetchContent_Declare(
        rapidjson
        GIT_REPOSITORY "https://github.com/Tencent/rapidjson.git"
        GIT_TAG master
)
FetchContent_MakeAvailable(rapidjson)
include_directories("${rapidjson_SOURCE_DIR}/include")