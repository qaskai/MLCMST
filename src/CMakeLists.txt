file(
    GLOB_RECURSE MLCMST_SRC
    "*.cpp"
)

add_library(MLCMST ${MLCMST_SRC})
