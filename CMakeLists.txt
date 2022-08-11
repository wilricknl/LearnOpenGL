cmake_minimum_required(VERSION 3.21)
project(LearnOpenGL
        VERSION 0.1.0
        DESCRIPTION "Cross-platform follow along with LearnOpenGL"
        LANGUAGES CXX)

configure_file(include/config.hpp.in config.hpp)

set(CMAKE_CXX_STANDARD 17)

if (MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

include_directories(include ${GLAD_INCLUDE_DIRS})

find_package(glad REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

add_subdirectory(hello_window)
add_subdirectory(triangle)
