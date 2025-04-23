project(tertis)

include_directories(include)
add_executable(${PROJECT_NAME}
    main.cpp
    tetris_game.cpp
    level.cpp
    blocks.cpp
)