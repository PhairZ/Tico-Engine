cmake_minimum_required(VERSION 3.22)

project(tico-engine)

add_subdirectory(core)
link_libraries(tico)

# Games (Comment and uncomment to Build).
add_subdirectory(tetris)