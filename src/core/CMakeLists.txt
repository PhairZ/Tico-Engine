project(tico)

include_directories(include)
add_library(${PROJECT_NAME} SHARED
    data.cpp
    game.cpp
    input.cpp
    objects.cpp
    renderer.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    include
)