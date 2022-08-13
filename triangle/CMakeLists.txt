add_executable(triangle main.cpp)
target_include_directories(triangle PUBLIC ${PROJECT_BINARY_DIR})
target_link_libraries(triangle common glad::glad glfw OpenGL::GL)

# Copy required files to build folder after building
# https://thomas.trocha.com/blog/cmake--copy-files-after-build/
add_custom_command(
        TARGET triangle POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/animated.frag
            ${CMAKE_CURRENT_BINARY_DIR}/animated.frag
        COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/interpolated.frag
            ${CMAKE_CURRENT_BINARY_DIR}/interpolated.frag
        COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/shader.vert
            ${CMAKE_CURRENT_BINARY_DIR}/shader.vert
)
