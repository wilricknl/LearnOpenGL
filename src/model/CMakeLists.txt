add_executable(model main.cpp)
target_include_directories(model PUBLIC ${PROJECT_BINARY_DIR})
target_link_libraries(model common glad::glad glfw glm::glm OpenGL::GL)

add_custom_command(
        TARGET model POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/grid.frag
        ${CMAKE_CURRENT_BINARY_DIR}/grid.frag
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/grid.vert
        ${CMAKE_CURRENT_BINARY_DIR}/grid.vert
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/shader.frag
        ${CMAKE_CURRENT_BINARY_DIR}/shader.frag
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/shader.vert
        ${CMAKE_CURRENT_BINARY_DIR}/shader.vert
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/assets/models/backpack
        ${CMAKE_CURRENT_BINARY_DIR}/backpack/
)

