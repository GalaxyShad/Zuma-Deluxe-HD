cmake_minimum_required(VERSION 3.15)
project(
    ZumaHD
    LANGUAGES C
    VERSION 0.1.0
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_library (isBassFound bass NO_CACHE PATHS ${PROJECT_SOURCE_DIR}/lib/bass/c/x64)
if (${isBassFound} STREQUAL isBassFound-NOTFOUND)
    message(STATUS "Downloading bass...")
    file(DOWNLOAD "https://www.un4seen.com/files/bass24.zip" "lib/bass-win.zip" SHOW_PROGRESS)
    file(ARCHIVE_EXTRACT INPUT "lib/bass-win.zip" DESTINATION lib/bass)
endif()


find_library (isBassFxFound bass_fx NO_CACHE PATHS ${PROJECT_SOURCE_DIR}/lib/bass-fx/C/x64)
if (${isBassFxFound} STREQUAL isBassFxFound-NOTFOUND)
    message(STATUS "Downloading bass fx...")
    file(DOWNLOAD "https://www.un4seen.com/files/z/0/bass_fx24.zip" "lib/bass-fx-win.zip" SHOW_PROGRESS)
    file(ARCHIVE_EXTRACT INPUT "lib/bass-fx-win.zip" DESTINATION lib/bass-fx)
endif()

include(FetchContent)

message("Fetching expat")
FetchContent_Declare(
    expat
    GIT_REPOSITORY https://github.com/libexpat/libexpat.git
    GIT_TAG        d8f43d6757af5ca5aae6cc185d6481f37f6c3be2
)

FetchContent_MakeAvailable(expat)

set(EXPAT_BUILD_DOCS OFF)
set(EXPAT_BUILD_TOOLS OFF)
add_subdirectory(${expat_SOURCE_DIR}/expat ${expat_BINARY_DIR})

message("Fetching sdl2")
FetchContent_Declare(
    sdl2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        7914234b2601eeb2c6563462649f269e8cfbb16c
)

FetchContent_MakeAvailable(sdl2)

message("Fetching sdl2 image")
FetchContent_Declare(
    sdl2_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG        c1b7cf959c7d3150b0707f95a0c5268689c9e537
)

set(SDL2IMAGE_SAMPLES OFF)
FetchContent_MakeAvailable(sdl2_image)

# message("Fetching SDL2_ttf")
# FetchContent_Declare(
#     SDL2_TTF
#     GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
#     GIT_TAG        52f81247c52ccd1161523081736c2ccb738078bd
# )
# FetchContent_MakeAvailable(SDL2_TTF)

# 
FILE(GLOB_RECURSE SOURCES RELATIVE ${PROJECT_SOURCE_DIR} "src/*.c")

add_executable(
    ZumaHD
    ${SOURCES}
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
    "D:/workspace/zuma-hd/code/lib/SDL2_ttf-2.20.0/x86_64-w64-mingw32/include"
    # "${SDL2_TTF_SOURCE_DIR}"
)

link_directories(
    "D:/workspace/zuma-hd/code/lib/SDL2_ttf-2.20.0/x86_64-w64-mingw32/lib"
    ${PROJECT_SOURCE_DIR}/lib/bass/c
    ${PROJECT_SOURCE_DIR}/lib/bass-fx/C
)

target_link_libraries(
    ZumaHD PRIVATE
    SDL2
    SDL2_Image

    # "D:/workspace/zuma-hd/code/lib/SDL2_ttf-2.20.0/x86_64-w64-mingw32/lib/libSDL2_ttf.a"
    "D:/workspace/zuma-hd/code/lib/SDL2_ttf-2.20.0/x86_64-w64-mingw32/SDL2_ttf.lib"

    ${isBassFound}
    ${isBassFxFound}
    
    expat
)

# Copy DLLs needed for runtime on Windows
if (WIN32)
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
