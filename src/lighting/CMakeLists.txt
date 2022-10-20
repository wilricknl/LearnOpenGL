add_executable(lighting main.cpp)
target_include_directories(lighting PUBLIC ${PROJECT_BINARY_DIR})
target_link_libraries(lighting common glad::glad glfw glm::glm OpenGL::GL)

add_custom_command(
        TARGET lighting POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/light.frag
            ${CMAKE_CURRENT_BINARY_DIR}/light.frag
        COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/light.vert
            ${CMAKE_CURRENT_BINARY_DIR}/light.vert
        COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/shader.frag
            ${CMAKE_CURRENT_BINARY_DIR}/shader.frag
        COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/shader.vert
            ${CMAKE_CURRENT_BINARY_DIR}/shader.vert
        COMMAND ${CMAKE_COMMAND} -E copy
            ${PROJECT_SOURCE_DIR}/assets/textures/container2.png
            ${CMAKE_CURRENT_BINARY_DIR}/container2.png
        COMMAND ${CMAKE_COMMAND} -E copy
            ${PROJECT_SOURCE_DIR}/assets/textures/container2_specular.png
            ${CMAKE_CURRENT_BINARY_DIR}/container2_specular.png
        COMMAND ${CMAKE_COMMAND} -E copy
        ${PROJECT_SOURCE_DIR}/assets/textures/matrix.jpg
        ${CMAKE_CURRENT_BINARY_DIR}/matrix.jpg
)
