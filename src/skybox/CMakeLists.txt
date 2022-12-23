add_executable(skybox main.cpp)
target_include_directories(skybox PUBLIC ${PROJECT_BINARY_DIR})
target_link_libraries(skybox common glad::glad glfw glm::glm OpenGL::GL)

add_custom_command(
    TARGET skybox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/outline.frag
        ${CMAKE_CURRENT_BINARY_DIR}/outline.frag
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/shader.frag
        ${CMAKE_CURRENT_BINARY_DIR}/shader.frag
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/shader.vert
        ${CMAKE_CURRENT_BINARY_DIR}/shader.vert
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/skybox.frag
        ${CMAKE_CURRENT_BINARY_DIR}/skybox.frag
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/skybox.vert
        ${CMAKE_CURRENT_BINARY_DIR}/skybox.vert
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/assets/models/cube
        ${CMAKE_CURRENT_BINARY_DIR}/cube
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/assets/models/grass
        ${CMAKE_CURRENT_BINARY_DIR}/grass/
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/assets/models/plane
        ${CMAKE_CURRENT_BINARY_DIR}/plane/
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/assets/skybox
        ${CMAKE_CURRENT_BINARY_DIR}/skybox/
)
