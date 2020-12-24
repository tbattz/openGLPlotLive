project(examplePlot)

# Create the exectuable
add_executable(${PROJECT_NAME} examplePlot.cpp)

# Link the static library from subproject 1
target_link_libraries(${PROJECT_NAME} PRIVATE openGLPlotLive ${LIBS})

# Copy Shader files to build directory
add_custom_command(TARGET examplePlot PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/../Shaders $<TARGET_FILE_DIR:examplePlot>/Shaders)

# Copy Texture files to build directory
add_custom_command(TARGET examplePlot PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/../textures $<TARGET_FILE_DIR:examplePlot>/textures)