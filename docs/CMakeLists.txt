## Based on https://devblogs.microsoft.com/cppblog/clear-functional-c-documentation-with-sphinx-breathe-doxygen-cmake/

find_package(Doxygen)
find_package(Sphinx)

# Find the headers
get_target_property(OPENGLPLOTLIVE_PUBLIC_HEADER_DIR openGLPlotLive INTERFACE_INCLUDE_DIRECTORIES)
file(GLOB_RECURSE OPENGLPLOTLIVE_PUBLIC_HEADERS ${OPENGLPLOTLIVE_PUBLIC_HEADER_DIR}/*.h)

# Doxygen variables
set(DOXYGEN_INPUT_DIR ${PROJECT_SOURCE_DIR}/src/)
set(DOXYGEN_OUTPUT_DIR ${PROJECT_SOURCE_DIR}/docs/doxygen/)
set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/xml/index.xml)
set(DOXYFILE_IN ${PROJECT_SOURCE_DIR}/docs/Doxyfile.in)
set(DOXYFILE_OUT ${PROJECT_SOURCE_DIR}/docs/doxygen/Doxyfile)

# Replace variables inside @@ witht he current values
configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

# Create directory if required
file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})

# Only regenerate doxygen when the doxyfile or public headers change
add_custom_command(OUTPUT ${DOXYGEN_INDEX_FILE}
        DEPENDS ${OPENGLPLOTLIVE_PUBLIC_HEADERS}
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
        MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}
        COMMENT "Generating documentation with doxygen"
        VERBATIM)

# Target to run the job
add_custom_target(doxygen ALL DEPENDS ${DOXYGEN_INDEX_FILE})



# Sphinx variables
set(SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
set(SPHINX_BUILD ${PROJECT_SOURCE_DIR}/docs/sphinx)
set(SPHINX_INDEX_FILE ${SPHINX_BUILD}/index.html)

# Only regenerate Sphinx when:
# - Doxygen has rerun
# - Our doc files have been updated
# - The Sphinx config has been updated
add_custom_command(OUTPUT ${SPHINX_INDEX_FILE}
        COMMAND
        ${SPHINX_EXECUTABLE} -b html
        # Tell Breathe where to find the Doxygen output
        -Dbreathe_projects.openGLPlotLive=${DOXYGEN_OUTPUT_DIR}/xml
        ${SPHINX_SOURCE} ${SPHINX_BUILD}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS
        # Other docs files you want to track should go here (or in some variable)
        doxygen
        ${CMAKE_CURRENT_SOURCE_DIR}/index.rst
        ${DOXYGEN_INDEX_FILE}
        MAIN_DEPENDENCY ${SPHINX_SOURCE}/conf.py
        COMMENT "Generating documentation with Sphinx")

# Nice named target so we can run the job easily
add_custom_target(sphinx ALL DEPENDS ${SPHINX_INDEX_FILE})

# Add target that depends on both
add_custom_target(docs)
add_dependencies(docs doxygen sphinx)