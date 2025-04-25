project(music_maker)

add_executable(${PROJECT_NAME}
    main.cpp
    music_maker.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE include)