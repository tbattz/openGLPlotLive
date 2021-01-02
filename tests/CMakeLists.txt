# Define test executable name
set(BINARY ${CMAKE_PROJECT_NAME}_tests)

# Find source files
file(GLOB TEST_SOURCES LIST_DIRECTORIES true *.h *.cpp)
set(SOURCES ${TEST_SOURCES})

# Add an executable target
add_executable(${BINARY} ${TEST_SOURCES} ${CMAKE_SOURCE_DIR}/include/glad.c)

# Specify libraries to be linked
target_link_libraries(${BINARY} PUBLIC gtest openGLPlotLive ${LIBS})

# Add tests to be run
add_test(NAME ${BINARY} COMMAND ${BINARY})

# Copy Shader files to build directory
add_custom_command(TARGET openGLPlotLive-proj_tests PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Shaders $<TARGET_FILE_DIR:openGLPlotLive>/../tests/Shaders)
add_custom_command(TARGET openGLPlotLive-proj_tests PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/textures $<TARGET_FILE_DIR:openGLPlotLive>/../tests/../textures)

# Add test target
add_custom_target(tests)
add_dependencies(tests openGLPlotLive-proj_tests)

