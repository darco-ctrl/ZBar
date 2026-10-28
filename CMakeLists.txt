
cmake_minimum_required(VERSION 3.21)

project(ZBar LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT WIN32)
    message(FATAL_ERROR "ZBar is only supported on Windows.")
endif()

find_package(Slint QUIET)

if(NOT Slint_FOUND)
    message(STATUS "Slint not found. Fetching it from Git...")

    include(FetchContent)

    FetchContent_Declare(
        Slint
        GIT_REPOSITORY https://github.com/slint-ui/slint.git
        GIT_TAG release/1
        SOURCE_SUBDIR api/cpp
    )

    FetchContent_MakeAvailable(Slint)
endif()


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
add_custom_target(copy_zbar_configs ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/src/configs/defaults"
        "$<TARGET_FILE_DIR:zbar>/configs/defaults"
    VERBATIM
)
add_dependencies(copy_zbar_configs zbar)
target_link_libraries(zbar PRIVATE Slint::Slint)

# slint_target_sources()

# Copy required runtime DLLs beside the executable.
add_custom_command(
    TARGET zbar
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_RUNTIME_DLLS:zbar>
        $<TARGET_FILE_DIR:zbar>
    COMMAND_EXPAND_LISTS
)
