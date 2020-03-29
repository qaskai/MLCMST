file(
    GLOB_RECURSE generation_SRC
    "*.cpp"
)

add_library(generation ${generation_SRC})
target_link_libraries(generation PUBLIC model)