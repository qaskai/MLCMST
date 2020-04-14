file(
    GLOB_RECURSE MLCMST_SRC
    "*.cpp"
)

add_library(mlcmst ${MLCMST_SRC})
