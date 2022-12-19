find_package(freetype CONFIG REQUIRED)

add_executable(text main.cpp)
target_include_directories(text PUBLIC ${PROJECT_BINARY_DIR})
target_link_libraries(text common freetype glad::glad glfw OpenGL::GL)

add_custom_command(
	TARGET text POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
		${CMAKE_CURRENT_SOURCE_DIR}/text.frag
		${CMAKE_CURRENT_BINARY_DIR}/text.frag
	COMMAND ${CMAKE_COMMAND} -E copy
		${CMAKE_CURRENT_SOURCE_DIR}/text.vert
		${CMAKE_CURRENT_BINARY_DIR}/text.vert
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		${PROJECT_SOURCE_DIR}/assets/fonts/
		${CMAKE_CURRENT_BINARY_DIR}/assets/fonts/
)
