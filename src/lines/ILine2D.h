//
// Created by bcub3d-desktop on 27/3/20.
//

#ifndef OPENGLPLOTLIVE_ILINE2D_H
#define OPENGLPLOTLIVE_ILINE2D_H

// GLFW (Multi-platform library for OpenGL)
#include <GLFW/glfw3.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Project Includes
#include "../rendering/shader.h"
#include "lineColours.h"

// Standard Includes
#include <vector>


namespace GLPL {
    class ILine2D {
    public:
        ILine2D();

        void createAndSetupBuffers(GLuint *VAOPt, GLuint *VBOPt, int dataSizeBytes, const void *dataAddress,
                                   int strideBytes, int glType=GL_FLOAT);

        void drawData(Shader shader, glm::mat4 axesLimitViewportTrans, GLuint *VAOPt, glm::vec3 colour, int nPts,
                      GLenum mode);
        void setLineColour(glm::vec3 lineColor);
        void setMode(GLenum newMode);
        glm::vec3 getColour();
        GLenum getMode();

        virtual void Draw(Shader shader, glm::mat4 axesLimitViewportTrans) = 0;

        virtual std::vector<float> getMinMax() = 0;

    private:
        /* Attributes */
        glm::vec3 colour = LC_WHITE;
        GLenum mode; // Mode, line or points
    };
}


#endif //OPENGLPLOTLIVE_ILINE2D_H
