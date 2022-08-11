add_executable(triangle main.cpp)
target_include_directories(triangle PUBLIC ${PROJECT_BINARY_DIR})
target_link_libraries(triangle glad::glad glfw OpenGL::GL)
