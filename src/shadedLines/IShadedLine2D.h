//
// Created by tbatt on 12/04/2020.
//

#ifndef OPENGLPLOTLIVE_PROJ_ISHADEDLINE2D_H
#define OPENGLPLOTLIVE_PROJ_ISHADEDLINE2D_H

// GLFW (Multi-platform library for OpenGL)
#include <GLFW/glfw3.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Project Includes
#include "../rendering/shader.h"
#include "../lines/lineColours.h"
#include "../axes/Plotable.h"
#include "../lines/ILine2D.h"

// Standard Includes
#include <vector>


namespace GLPL {
    class IShadedLine2D : public ILine2D {
    public:
        IShadedLine2D(std::shared_ptr<ParentDimensions> parentDimensions);

        void createAndSetupBuffers(int vertDataSizeBytes, int indicesDataSizeBytes,
                                   const void *vertDataAddress, const void *indicesDataAddress,
                                   int strideBytes, int glType=GL_FLOAT);
        void drawData(int numIndices, bool selected);
        int getHoverCursor();

    protected:
        // Line Buffers
        GLuint lineVAO, lineVBO, lineEBO;

        // Functions
        std::shared_ptr<Shader> selectShader();

    };
}

#endif //OPENGLPLOTLIVE_PROJ_ISHADEDLINE2D_H
