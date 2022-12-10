# cmake version
cmake_minimum_required(VERSION 3.10)

# Project Name
project(openGLPlotLive)

# Create library
add_library(${PROJECT_NAME} "")

# Glad
target_sources(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include/glad.c)
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Add subdirectories
add_subdirectory(axes)
add_subdirectory(dataTypes)
add_subdirectory(lines)
add_subdirectory(shadedLines)
add_subdirectory(plot)
add_subdirectory(rendering)
add_subdirectory(window)
add_subdirectory(texts)
add_subdirectory(shapes)
add_subdirectory(util)
add_subdirectory(interaction)
add_subdirectory(textures)
add_subdirectory(scatterPlot)

# Copy Shader files to build directory
add_custom_command(TARGET openGLPlotLive PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/../Shaders $<TARGET_FILE_DIR:openGLPlotLive>/Shaders)
add_custom_command(TARGET openGLPlotLive PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/../textures $<TARGET_FILE_DIR:openGLPlotLive>/../textures)



