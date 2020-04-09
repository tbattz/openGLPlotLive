# cmake version
cmake_minimum_required(VERSION 3.0)
# Project Name
project(examplePlot)

# Set OpenGL Policy
set(OpenGL_GL_PREFERENCE "GLVND")

# Add c11++ flag to compiler
list(APPEND CMAKE_CXX_FLAGS "-std=c++11 -Wno-narrowing")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

# Add Cmake Module Path
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake/")


# Add Include Directory
if(WIN32)
	include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../Includes/")
	include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../Includes/freetype") # TODO - Check if this can be removed
elseif(UNIX)
	include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../Includes/")
endif(WIN32)

# Add Sources
file(GLOB SOURCES_RENDERING rendering/*.cpp rendering/*.h)
file(GLOB SOURCES_WINDOW window/*.cpp window/*.h)
file(GLOB SOURCES_PLOT plot/*.cpp plot/*.h)
file(GLOB SOURCES_AXES axes/*.cpp axes/*.h)
file(GLOB SOURCES_LINES lines/*.cpp lines/*.h)
file(GLOB SOURCES_TYPES dataTypes/*.cpp dataTypes/*.h)
set(SOURCES ${SOURCES_RENDERING} ${SOURCES_AXES} ${SOURCES_LINES} ${SOURCES_PLOT} ${SOURCES_WINDOW} ${SOURCES_TYPES} ${CMAKE_CURRENT_SOURCE_DIR}/../Includes/glad.c)

if(WIN32)
	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Includes) # Todo - Check if this can be removed
elseif(UNIX)
endif(WIN32)

# Find Packages
find_package(GLM REQUIRED)
message(STATUS "GLM found: ${GLM_INCLUDE_DIR}")
find_package(FREETYPE REQUIRED)
MESSAGE(STATUS "Found FREETYPE: ${FREETYPE_LIBRARY}")
find_package(GLFW3 REQUIRED)
message(STATUS "Found Glfw3: ${GLFW3_LIBRARY}")
if(WIN32)
	set(FREETYPE_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/../Lib/freetype.a") # TODO - Check where this is created in the installer
ENDIF(WIN32)


# Setup Libraries
find_package(OpenGL REQUIRED)
add_definitions(${OPENGL_DEFINITIONS})
find_package(X11 REQUIRED)
if(WIN32)
	set(LIBS ${FREETYPE_LIBRARY} ${GLFW3_LIBRARY} z opengl32 )
elseif(UNIX)
	set(LIBS ${GLFW3_LIBRARY} X11 Xrandr Xinerama Xi Xxf86vm Xcursor dl pthread freetype GL)
endif(WIN32)

# Stop conflicting order of GLAD and GLFW
add_definitions(-DGLFW_INCLUDE_NONE)

# Create library for external use
MESSAGE( STATUS "Sources: " ${SOURCES})
add_library(openGLPlotLive STATIC ${SOURCES})

# Define example executable
add_executable(examplePlot examplePlot.cpp)
target_link_libraries(examplePlot openGLPlotLive ${LIBS})

# Copy Shader files to build directory
add_custom_command(TARGET examplePlot PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/../Shaders $<TARGET_FILE_DIR:examplePlot>/Shaders)

# Install to location
#install(TARGETS examplePlot DESTINATION ${CMAKE_SOURCE_DIR}/../bin)
#install(DIRECTORY ${CMAKE_SOURCE_DIR}/../Shaders DESTINATION ${CMAKE_SOURCE_DIR}/../bin)


