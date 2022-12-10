//
// Created by bcub3d-desktop on 27/3/20.
//

#ifndef OPENGLPLOTLIVE_ILINE2D2_H
#define OPENGLPLOTLIVE_ILINE2D2_H

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// GLFW (Multi-platform library for OpenGL)
#include <GLFW/glfw3.h>

// Project Includes
#include "../rendering/shader.h"
#include "lineColours.h"
#include "../axes/Plotable.h"
#include "ILine2D.h"

// Standard Includes
#include <vector>


namespace GLPL {
    class ISingleLine2D : public ILine2D {
    public:
        ISingleLine2D(std::shared_ptr<ParentDimensions> parentDimensions);

        void createAndSetupBuffers(int dataSizeBytes, const void *dataAddress,
                                   int strideBytes, int glType=GL_FLOAT);
        void createAndSetupBuffers(int vertDataSizeBytes, int indicesDataSizeBytes,
                                    const void *vertDataAddress, const void *indicesDataAddress,
                                    int strideBytes, int glType=GL_FLOAT);

        void createAndSetupLegendBuffers(int dataSizeBytes, const void *dataAddress,
                                         int strideBytes, int glType=GL_FLOAT);

        void drawData(int nPts, bool selected);

    protected:
        // Line Buffers
        GLuint lineVAO, lineVBO, lineEBO;
        GLuint legendLineVAO, legendLineVBO;

        // Functions
        std::shared_ptr<Shader> selectShader();

    };
}


#endif //OPENGLPLOTLIVE_ILINE2D2_H
