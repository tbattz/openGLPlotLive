# cmake version
cmake_minimum_required(VERSION 3.0)
# Project Name
project(openGLPlotLive)

# Set OpenGL Policy
set(OpenGL_GL_PREFERENCE "GLVND")

# Add c11++ flag to compiler
list(APPEND CMAKE_CXX_FLAGS "-std=c++11 -Wno-narrowing")

# Add Cmake Module Path
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../cmake/")

# Add Include Directory
if(WIN32)
	include_directories("${CMAKE_SOURCE_DIR}/../Includes/")
	include_directories("${CMAKE_SOURCE_DIR}/../Includes/freetype")
elseif(UNIX)
	include_directories("${CMAKE_SOURCE_DIR}/../Includes/")
endif(WIN32)

# Add Sources
file(GLOB SOURCES_RENDERING rendering/*.cpp rendering/*.h)
file(GLOB SOURCES_WINDOW window/*.cpp window/*.h)
file(GLOB SOURCES_PLOT plot/*.cpp plot/*.h)
file(GLOB SOURCES_AXES axes/*.cpp axes/*.h)
file(GLOB SOURCES_LINES lines/*.cpp lines/*.h)
file(GLOB SOURCES_TYPES dataTypes/*.cpp dataTypes/*.h)
set(SOURCES ${SOURCES_RENDERING} ${SOURCES_AXES} ${SOURCES_LINES} ${SOURCES_PLOT} ${SOURCES_WINDOW} ${SOURCES_TYPES} ../Includes/glad.c)

if(WIN32)
	link_directories(${CMAKE_SOURCE_DIR}/../Includes)
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
	set(FREETYPE_LIBRARY "${CMAKE_SOURCE_DIR}/../Lib/freetype.a")
ENDIF(WIN32)


# Set Flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
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
add_library(openGLPlotLiveLib STATIC ${SOURCES})

# Define executables
MESSAGE( STATUS "Sources: " ${SOURCES})
add_executable(openGLPlotLive examplePlot.cpp)
target_link_libraries(openGLPlotLive openGLPlotLiveLib ${LIBS})

# Copy Shader files to build directory
add_custom_command(TARGET openGLPlotLive PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/../Shaders $<TARGET_FILE_DIR:openGLPlotLive>/Shaders)


