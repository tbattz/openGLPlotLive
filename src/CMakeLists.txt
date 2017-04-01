# cmake version
cmake_minimum_required(VERSION 3.0)
# Project Name
project(openGLPlotLive)

# Add c11++ flag to compiler
list(APPEND CMAKE_CXX_FLAGS "-std=c++11 -Wno-narrowing")

# Add Cmake Module Path
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../cmake/")

# Add Include Directory
if(WIN32)
	include_directories("${CMAKE_SOURCE_DIR}/../Includes/")
	include_directories("${CMAKE_SOURCE_DIR}/../Includes/freetype")
elseif(UNIX)
	include_directories(/usr/local/include/)
	include_directories(/usr/include/)
	include_directories(/usr/local/include/freetype/)
endif(WIN32)

# Add Sources
file(GLOB SOURCES "*.cpp" "*.h")

if(WIN32)
	link_directories(${CMAKE_SOURCE_DIR}/../Includes)
elseif(UNIX)
	link_directories(/usr/local/lib /usr/lib /usr/local)
endif(WIN32)

# Find Packages
find_package(GLM REQUIRED)
message(STATUS "GLM found: ${GLM_INCLUDE_DIR}")
find_package(FREETYPE REQUIRED)
MESSAGE(STATUS "Found FREETYPE: ${FREETYPE_LIBRARY}")
find_package(GLFW3 REQUIRED)
message(STATUS "Found Glfw3: ${GLFW3_LIBRARY}")	
find_package(GLEW REQUIRED)
message(STATUS "Found Glew: ${GLEW_LIBRARY}")
if(WIN32)
	set(GLEW_LIBRARY "${CMAKE_SOURCE_DIR}/../Lib/libglew32.a")
	set(FREETYPE_LIBRARY "${CMAKE_SOURCE_DIR}/../Lib/freetype.a")
ENDIF(WIN32)


# Set Flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
find_package(OpenGL REQUIRED)
add_definitions(${OPENGL_DEFINITIONS})
find_package(X11 REQUIRED)
if(WIN32)
	set(LIBS ${FREETYPE_LIBRARY} ${GLFW3_LIBRARY} ${GLEW_LIBRARY} z opengl32 )
elseif(UNIX)
	set(LIBS ${GLFW3_LIBRARY} X11 Xrandr Xinerama Xi Xxf86vm Xcursor GL dl pthread GLEW freetype)
endif(WIN32)

# Define executables
MESSAGE( STATUS "Sources: " ${SOURCES} )
add_executable(openGLPlotLive ${SOURCES})
target_link_libraries(openGLPlotLive ${LIBS})


