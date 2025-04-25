project(tico)

include_directories(include)
add_library(${PROJECT_NAME} SHARED
    data.cpp
    game.cpp
    renderer.cpp
    input.cpp
    audio.cpp
    objects.cpp
)

if(WIN32)
    find_package(PortAudio REQUIRED)
elseif(APPLE)
    find_package(PortAudio REQUIRED)
else()
    include(FetchContent)
    FetchContent_Declare(
        portaudio
        URL https://github.com/PortAudio/portaudio/archive/refs/tags/v19.7.0.tar.gz
    )
    FetchContent_MakeAvailable(portaudio)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC
    include
)

target_link_libraries(${PROJECT_NAME} portaudio m)