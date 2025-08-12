cmake_minimum_required(VERSION 3.10)
project(GameDev LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

include(FetchContent)

# SDL2
FetchContent_Declare(
  SDL2
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-2.30.1
)
FetchContent_MakeAvailable(SDL2)

# SDL2_ttf
FetchContent_Declare(
  SDL2_ttf
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
  GIT_TAG release-2.22.0
)
FetchContent_MakeAvailable(SDL2_ttf)

# SDL2_image
FetchContent_Declare(
  SDL2_img
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
  GIT_TAG release-2.8.2
)
FetchContent_MakeAvailable(SDL2_img)

# SDL2_gfx
FetchContent_Declare(
  SDL2_gfx
  GIT_REPOSITORY https://github.com/ferzkopp/SDL2_gfx.git
  GIT_TAG main
)
FetchContent_MakeAvailable(SDL2_gfx)

# Aggiungi SDL2_gfx manualmente come libreria statica
add_library(SDL2_gfx_lib STATIC
  ${sdl2_gfx_SOURCE_DIR}/SDL2_framerate.c
  ${sdl2_gfx_SOURCE_DIR}/SDL2_gfxPrimitives.c
  ${sdl2_gfx_SOURCE_DIR}/SDL2_rotozoom.c
)
target_include_directories(SDL2_gfx_lib PUBLIC ${sdl2_gfx_SOURCE_DIR})
target_link_libraries(SDL2_gfx_lib SDL2::SDL2 m)

# Raccogli tutti i file sorgente .cpp ricorsivamente
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Crea l'eseguibile principale
add_executable(GameDev ${SOURCES})

add_custom_command(TARGET GameDev POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:GameDev>/assets"
  COMMENT "Copying assets to runtime dir"
)

# Includi le directory necessarie
target_include_directories(GameDev PRIVATE
  ${SDL2_SOURCE_DIR}/include
  ${SDL2_ttf_SOURCE_DIR}/include
  ${SDL2_img_SOURCE_DIR}/include
  ${sdl2_gfx_SOURCE_DIR}
  src/headers
)

# Collega tutte le librerie
target_link_libraries(GameDev
  SDL2::SDL2
  SDL2_ttf::SDL2_ttf
  SDL2_image::SDL2_image
  SDL2_gfx_lib
)