include(FetchContent)

FetchContent_Declare(
        or-tools
        GIT_REPOSITORY "https://github.com/google/or-tools.git"
        GIT_TAG master
)
FetchContent_MakeAvailable(or-tools)

FetchContent_Declare(
        glog
        GIT_REPOSITORY "https://github.com/google/glog.git"
        GIT_TAG master
)
FetchContent_MakeAvailable(glog)

FetchContent_Declare(
        catch2
        GIT_REPOSITORY "https://github.com/catchorg/Catch2.git"
        GIT_TAG master
)
FetchContent_MakeAvailable(catch2)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${catch2_SOURCE_DIR}/contrib")
