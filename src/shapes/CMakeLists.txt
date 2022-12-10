# Add sources
file(GLOB CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB H_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
target_sources(${PROJECT_NAME} PRIVATE ${CPP_FILES})
target_sources(${PROJECT_NAME} PUBLIC "${H_FILES}")

# Setup include directories
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/shapes)