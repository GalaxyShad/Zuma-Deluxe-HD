cmake_minimum_required(VERSION 3.15)
project(
    ZumaHD
    LANGUAGES C
    VERSION 0.1.0
)
include(FetchContent)

SET(CMAKE_C_COMPILER clang)
set(CMAKE_C_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


if(UNIX)
set(BASS_LIB_NAME "bass24.zip")
endif()
if (WIN32)
set(BASS_LIB_NAME "bass-linux.zip")
endif()

message("Fetching bass")
find_library (isBassFound bass NO_CACHE PATHS ${PROJECT_SOURCE_DIR}/lib/bass/libs/x86_64)
if (${isBassFound} STREQUAL isBassFound-NOTFOUND)
    message(STATUS "Downloading bass...")
    file(DOWNLOAD "https://www.un4seen.com/files/${BASS_LIB_NAME}" "lib/bass.zip" SHOW_PROGRESS)
    file(ARCHIVE_EXTRACT INPUT "lib/bass.zip" DESTINATION lib/bass)
endif()

find_library (isBassFxFound bass_fx NO_CACHE PATHS ${PROJECT_SOURCE_DIR}/lib/bass-fx/libs/x86_64)
if (${isBassFxFound} STREQUAL isBassFxFound-NOTFOUND)
    message(STATUS "Downloading bass fx...")
    file(DOWNLOAD "https://www.un4seen.com/files/z/0/bass_fx24.zip" "lib/bass-fx.zip" SHOW_PROGRESS)
    file(ARCHIVE_EXTRACT INPUT "lib/bass-fx.zip" DESTINATION lib/bass-fx)
endif()

if(UNIX)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(EXPAT REQUIRED)
endif()
if(WIN32)


    message("Fetching expat")
    FetchContent_Declare(
    expat
    URL https://github.com/libexpat/libexpat/archive/refs/tags/R_2_4_8.zip
    FIND_PACKAGE_ARGS
    )

    FetchContent_MakeAvailable(expat)

    find_library(EXPAT)

    set(EXPAT_BUILD_DOCS OFF)
    set(EXPAT_BUILD_TOOLS OFF)
    add_subdirectory(${expat_SOURCE_DIR}/expat ${expat_BINARY_DIR})

    message("Fetching sdl2")
    FetchContent_Declare(
    sdl2
    URL https://github.com/libsdl-org/SDL/archive/refs/tags/release-2.24.0.zip
    FIND_PACKAGE_ARGS
    )

    FetchContent_MakeAvailable(sdl2)

    message("Fetching sdl2 image")
    FetchContent_Declare(
    sdl2_image
    URL https://github.com/libsdl-org/SDL_image/archive/refs/tags/release-2.6.2.zip
    FIND_PACKAGE_ARGS
    )

    set(SDL2IMAGE_SAMPLES OFF)
    FetchContent_MakeAvailable(sdl2_image)
endif()

add_executable(
    ZumaHD
    "src/main.c"
    "src/Application.c"
    "src/menu/MenuMgr.c"
    "src/global/Animation.c"
    "src/global/Engine.c"
    "src/gameplay/Game.c"
    "src/gameplay/Level.c"
    "src/gameplay/objects/BallChain.c"
    "src/gameplay/objects/Bullet.c"
    "src/gameplay/objects/Frog.c"
    "src/gameplay/objects/FX.c"
)

target_include_directories(
    ZumaHD PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/src"
    "${PROJECT_SOURCE_DIR}/lib/bass/c"
    "${PROJECT_SOURCE_DIR}/lib/bass-fx/C"

    "${expat_SOURCE_DIR}/expat/lib"
    "${sdl2_SOURCE_DIR}/include"
    "${sdl2_image_SOURCE_DIR}"
)

link_directories(
    ${PROJECT_SOURCE_DIR}/lib/bass/libs/x86_64/
    ${PROJECT_SOURCE_DIR}/lib/bass-fx/libs/x86_64/
    )

target_link_libraries(
    ZumaHD PRIVATE
    SDL2
    SDL2_image
    m
    expat
    bass
    bass_fx
    ${isBassFound}
    ${isBassFxFound}
    )

# Copy DLLs needed for runtime on Windows
if(WIN32)
    if(BUILD_SHARED_LIBS)
        add_custom_command(TARGET ZumaHD POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${PROJECT_SOURCE_DIR}/lib/bass/x64/bass.dll
            ${PROJECT_SOURCE_DIR}/lib/bass-fx/x64/bass_fx.dll
            $<TARGET_FILE_DIR:ZumaHD>)
    endif()
endif()

add_custom_command(TARGET ${PROJECT_NAME}  POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/content
                ${CMAKE_CURRENT_BINARY_DIR}/bin)
