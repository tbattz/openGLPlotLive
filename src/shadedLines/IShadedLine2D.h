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

// Standard Includes
#include <vector>


namespace GLPL {
    class IShadedLine2D {
    public:
        IShadedLine2D();

        void createAndSetupBuffers(GLuint *VAOPt, GLuint *VBOPt, GLuint *EBOPt,
                                   int vertDataSizeBytes, int indicesDataSizeBytes,
                                   const void *vertDataAddress, const void *indicesDataAddress,
                                   int strideBytes, int glType=GL_FLOAT);
        void drawData(Shader shader, glm::mat4 axesLimitViewportTrans, GLuint *VAOPt, glm::vec3 colour,
                int numIndices, GLenum mode);
        void setShadeColour(glm::vec3 shadeColour);
        void setMode(GLenum newMode);
        glm::vec3 getColour();
        GLenum getMode();

        virtual void Draw(Shader shader, glm::mat4 axesLimitViewportTrans) = 0;

        virtual std::vector<float> getMinMax() = 0;

    private:
        /* Data */
        glm::vec3 colour = LC_WHITE;
        GLenum mode; // Mode, shaded or line
    };
}

#endif //OPENGLPLOTLIVE_PROJ_ISHADEDLINE2D_H
