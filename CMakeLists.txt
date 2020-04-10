# Set minimum cmake
cmake_minimum_required(VERSION 3.10)

# Setup project
project(openGLPlotLive-proj)

# Set OpenGL Policy
set(OpenGL_GL_PREFERENCE "GLVND")

# Add c11++ flag to compiler
list(APPEND CMAKE_CXX_FLAGS "-std=c++11 -Wno-narrowing")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

# Add Cmake Module Path
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/")


# Find Packages
# GLM
find_package(GLM REQUIRED)
message(STATUS "GLM found: ${GLM_INCLUDE_DIR}")
# FREETYPE
find_package(FREETYPE REQUIRED)
MESSAGE(STATUS "Found FREETYPE: ${FREETYPE_LIBRARY}")
if(WIN32)
    set(FREETYPE_LIBRARY "${CMAKE_CURRENT_LIST_DIR}/../Lib/freetype.a") # TODO - Check where this is created in the installer
ENDIF(WIN32)
# GLFW3
find_package(GLFW3 REQUIRED)
message(STATUS "Found Glfw3: ${GLFW3_LIBRARY}")
# OpenGL
find_package(OpenGL REQUIRED)
add_definitions(${OPENGL_DEFINITIONS})
# X11
find_package(X11 REQUIRED)


# Set libraries
if(WIN32)
    set(LIBS ${FREETYPE_LIBRARY} ${GLFW3_LIBRARY} z opengl32 )
elseif(UNIX)
    set(LIBS ${GLFW3_LIBRARY} X11 Xrandr Xinerama Xi Xxf86vm Xcursor dl pthread freetype GL)
endif(WIN32)

# Stop conflicting order of GLAD and GLFW
add_definitions(-DGLFW_INCLUDE_NONE)

# Add subdirectories
add_subdirectory(src)
add_subdirectory(apps)
