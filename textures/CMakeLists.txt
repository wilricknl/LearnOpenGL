add_executable(textures main.cpp)
target_include_directories(textures PUBLIC ${PROJECT_BINARY_DIR})
target_link_libraries(textures common glad::glad glfw OpenGL::GL)

add_custom_command(
        TARGET textures POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/shader.frag
            ${CMAKE_CURRENT_BINARY_DIR}/shader.frag
        COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/shader.vert
            ${CMAKE_CURRENT_BINARY_DIR}/shader.vert
        COMMAND ${CMAKE_COMMAND} -E copy
            ${PROJECT_SOURCE_DIR}/assets/textures/container.jpg
            ${CMAKE_CURRENT_BINARY_DIR}/container.jpg
)
