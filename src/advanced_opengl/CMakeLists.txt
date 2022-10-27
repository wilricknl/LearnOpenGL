add_executable(advanced_opengl main.cpp)
target_include_directories(advanced_opengl PUBLIC ${PROJECT_BINARY_DIR})
target_link_libraries(advanced_opengl common glad::glad glfw glm::glm OpenGL::GL)
