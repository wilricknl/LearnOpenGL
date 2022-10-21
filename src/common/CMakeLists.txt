find_package(assimp CONFIG REQUIRED)

add_library(common camera.cpp mesh.cpp model.cpp shader.cpp stb_image.cpp texture.cpp)
target_include_directories(common PUBLIC ${PROJECT_BINARY_DIR})
target_link_libraries(common assimp::assimp glad::glad glfw glm::glm  OpenGL::GL)
