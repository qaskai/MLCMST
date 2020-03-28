file(
    GLOB model_SRC
    "*.cpp"
)

add_library(model_lib ${model_SRC})