add_executable(hello_window main.cpp)
target_include_directories(hello_window PUBLIC ${PROJECT_BINARY_DIR})
target_link_libraries(hello_window glad::glad glfw OpenGL::GL)
