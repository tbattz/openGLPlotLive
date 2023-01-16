project(apps)

# Resolves issue with unable to debug with CLion, reverts back to g++9
add_compile_options(-gdwarf-4)

# Create the exectuable
add_executable(examplePlot examplePlot.cpp)
add_executable(movingTimeframe movingTimeframe.cpp)


# Link the static library from subproject 1
target_link_libraries(examplePlot PRIVATE openGLPlotLive ${LIBS})
target_link_libraries(movingTimeframe PRIVATE openGLPlotLive ${LIBS})

# Copy Shader files to build directory
add_custom_command(TARGET examplePlot PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/../Shaders $<TARGET_FILE_DIR:examplePlot>/Shaders)

# Copy Texture files to build directory
add_custom_command(TARGET examplePlot PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/../textures $<TARGET_FILE_DIR:examplePlot>/textures)