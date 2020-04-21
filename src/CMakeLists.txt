file(
    GLOB_RECURSE MLCMST_SRC
    "*.cpp"
)

add_library(mlcmst ${MLCMST_SRC})
target_compile_features(mlcmst PUBLIC cxx_std_17)
target_link_libraries(mlcmst PUBLIC ortools::ortools)
