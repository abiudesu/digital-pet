cmake_minimum_required(VERSION 3.20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(
    grogue
    VERSION 0.1.0
    LANGUAGES CXX C
)

# fetch SDL under Windows
# for Appveyor CI/CD
if (WIN32)
    include(cmake/fetch_sdl.cmake)
    option(FETCH_SDL "fetch SDL2 SDL2_image SDL2_ttf SDL2_mixer from web" FALSE)
    if (FETCH_SDL)
        FetchSDL()
    endif()
endif()

include(cmake/utility.cmake)

# find SDL
include(cmake/FindSDL2_image.cmake)
include(cmake/FindSDL2_mixer.cmake)
include(cmake/FindSDL2_ttf.cmake)
include(cmake/FindSDL2.cmake)

# build game
aux_source_directory(src SRC)

add_executable(digital_pet ${SRC})
target_link_libraries(digital_pet PUBLIC SDL2 SDL2_image SDL2_ttf SDL2_mixer)
