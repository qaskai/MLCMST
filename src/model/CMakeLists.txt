file(
    GLOB model_SRC
    "*.cpp"
)

add_library(model ${model_SRC})