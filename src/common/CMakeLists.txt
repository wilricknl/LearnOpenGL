add_library(common camera.cpp shader.cpp stb_image.cpp)
target_include_directories(common PUBLIC ${PROJECT_BINARY_DIR})
target_link_libraries(common glad::glad glfw glm::glm  OpenGL::GL)
