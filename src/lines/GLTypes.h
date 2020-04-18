//
// Created by tbatt on 18/04/2020.
//

#ifndef IRACINGTRACEOVERLAY_PROJ_GLTYPES_H
#define IRACINGTRACEOVERLAY_PROJ_GLTYPES_H

// GLAD - Multi Language GL Loader-Generator
#include <glad/glad.h>

// GLFW (Multi-platform library for OpenGL)
#include <GLFW/glfw3.h>


namespace GLPL {
    // Template the vector type
    // Set mappings from Type T to GL Types
    template<class T>
    constexpr int getGLType();

    template<>
    constexpr int getGLType<int>() { return GL_INT; }

    template<>
    constexpr int getGLType<float>() { return GL_FLOAT; }

    template<>
    constexpr int getGLType<double>() { return GL_DOUBLE; }

}

#endif //IRACINGTRACEOVERLAY_PROJ_GLTYPES_H
