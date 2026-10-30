
cmake_minimum_required(VERSION 3.21)

project(ZBar LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT WIN32)
    message(FATAL_ERROR "ZBar is only supported on Windows.")
endif()

include(FetchContent)

# Slint
find_package(Slint QUIET)

if(NOT Slint_FOUND)
    message(STATUS "Slint not found. Fetching it from Git...")

    FetchContent_Declare(
        Slint
        GIT_REPOSITORY https://github.com/slint-ui/slint.git
        GIT_TAG release/1
        SOURCE_SUBDIR api/cpp
    )

    FetchContent_MakeAvailable(Slint)
endif()

# Glaze
FetchContent_Declare(
    glaze
    GIT_REPOSITORY https://github.com/stephenberry/glaze.git
    GIT_TAG v9.0.0
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(glaze)

# ZBar sources
file(GLOB_RECURSE ZBAR_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(zbar ${ZBAR_SOURCES})

target_compile_definitions(zbar PRIVATE
    ZBAR_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)

target_include_directories(zbar PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_link_libraries(zbar PRIVATE
    Slint::Slint
    glaze::glaze
)

# Copy default configs
add_custom_command(TARGET zbar POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/src/configs/defaults"
        "$<TARGET_FILE_DIR:zbar>/configs/defaults"
    VERBATIM
)

# Copy required runtime DLLs
add_custom_command(TARGET zbar POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_RUNTIME_DLLS:zbar>
        "$<TARGET_FILE_DIR:zbar>"
    COMMAND_EXPAND_LISTS
)
