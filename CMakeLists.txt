cmake_minimum_required(VERSION 4.0...4.4)
project(
    ZumaHD
    LANGUAGES C
    VERSION 0.1.0
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(EXPAT_BUILD_TESTS OFF)
set(EXPAT_BUILD_DOCS OFF)
set(EXPAT_BUILD_TOOLS OFF)

set(SDL2IMAGE_PNG ON)
set(SDL2IMAGE_JPG ON)

set(SDL2IMAGE_AVIF OFF)
set(SDL2IMAGE_BMP OFF)
set(SDL2IMAGE_GIF OFF)
set(SDL2IMAGE_JXL OFF)
set(SDL2IMAGE_LBM OFF)
set(SDL2IMAGE_PCX OFF)
set(SDL2IMAGE_PNM OFF)
set(SDL2IMAGE_QOI OFF)
set(SDL2IMAGE_SVG OFF)
set(SDL2IMAGE_TGA OFF)
set(SDL2IMAGE_TIF OFF)
set(SDL2IMAGE_WEBP OFF)
set(SDL2IMAGE_XCF OFF)
set(SDL2IMAGE_XPM OFF)
set(SDL2IMAGE_XV OFF)

add_subdirectory(thirdparty/expat/expat)
add_subdirectory(thirdparty/sdl2)
add_subdirectory(thirdparty/sdl2_image)

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
)

link_directories(
    ${PROJECT_SOURCE_DIR}/lib/bass/c
    ${PROJECT_SOURCE_DIR}/lib/bass-fx/C
)

target_link_libraries(
    ZumaHD PRIVATE
    SDL2::SDL2
    SDL2_image::SDL2_image
    expat
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/content
    ${CMAKE_CURRENT_BINARY_DIR}/bin)
