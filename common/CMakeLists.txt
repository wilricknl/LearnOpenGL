add_library(common shader.cpp)
target_include_directories(common PUBLIC ${PROJECT_BINARY_DIR})
target_link_libraries(common glad::glad glfw OpenGL::GL)
