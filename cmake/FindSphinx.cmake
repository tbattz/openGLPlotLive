if (UNIX)
    set(SPHINX_EXECUTABLE /usr/local/bin/sphinx-build)

elseif(LINUX)
    # Try and find sphinx-build
    find_program(SPHINX_EXECUTABLE
            NAMES sphinx-build
            DOC "Path to sphinx-build exectuable")

    include(FindPackageHandleStandardArgs)

    # Handle standard arguments for find_package
    find_package_handle_standard_args(Sphinx "Failed to find sphinx-build executable" SPHINX_EXECUTABLE)
endif()
message("-- SPHINX: " ${SPHINX_EXECUTABLE})
