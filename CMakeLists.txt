cmake_minimum_required(VERSION 4.0...4.4)
project(
    ZumaHD
    LANGUAGES C
    VERSION 0.1.0
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# deps exapt
set(EXPAT_BUILD_TESTS OFF)
set(EXPAT_BUILD_DOCS OFF)
set(EXPAT_BUILD_TOOLS OFF)

# deps sdl
add_subdirectory(thirdparty/expat/expat)
add_subdirectory(thirdparty/sdl2)

# project conf
FILE(GLOB_RECURSE SOURCES RELATIVE ${PROJECT_SOURCE_DIR} "src/*.c")

add_executable(
    ZumaHD
    ${SOURCES}
)

target_include_directories(
    ZumaHD PUBLIC

    "${PROJECT_SOURCE_DIR}/src"

    "${expat_SOURCE_DIR}/expat/lib"
    "${sdl2_SOURCE_DIR}/include"

    "${PROJECT_SOURCE_DIR}/thirdparty/stb"
)

target_link_libraries(
    ZumaHD PRIVATE
    SDL2::SDL2
    expat
)

# copy assets
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/content
    ${CMAKE_CURRENT_BINARY_DIR}/bin)
