project(tertis)

add_executable(${PROJECT_NAME}
    main.cpp
    tetris_game.cpp
    level.cpp
    blocks.cpp
)
target_include_directories(${PROJECT_NAME} PRIVATE include)