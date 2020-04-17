file(
    GLOB_RECURSE MLCMST_SRC
    "*.cpp"
)

add_library(mlcmst ${MLCMST_SRC})
target_link_libraries(mlcmst PUBLIC ortools::ortools)
