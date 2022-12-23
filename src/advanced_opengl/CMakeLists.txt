add_executable(advanced_opengl main.cpp)
target_include_directories(advanced_opengl PUBLIC ${PROJECT_BINARY_DIR})
target_link_libraries(advanced_opengl common glad::glad glfw glm::glm OpenGL::GL)

add_custom_command(
    TARGET advanced_opengl POST_BUILD
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
        ${CMAKE_CURRENT_SOURCE_DIR}/quad.frag
        ${CMAKE_CURRENT_BINARY_DIR}/quad.frag
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/quad.vert
        ${CMAKE_CURRENT_BINARY_DIR}/quad.vert
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/assets/models/cube
        ${CMAKE_CURRENT_BINARY_DIR}/cube
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/assets/models/grass
        ${CMAKE_CURRENT_BINARY_DIR}/grass/
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/assets/models/plane
        ${CMAKE_CURRENT_BINARY_DIR}/plane/
)
