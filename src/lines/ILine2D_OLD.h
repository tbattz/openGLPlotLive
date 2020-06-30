//
// Created by bcub3d-desktop on 27/3/20.
//

#ifndef OPENGLPLOTLIVE_ILINE2D_H
#define OPENGLPLOTLIVE_ILINE2D_H

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// GLFW (Multi-platform library for OpenGL)
#include <GLFW/glfw3.h>

// Project Includes
#include "../rendering/shader.h"
#include "lineColours.h"

// Standard Includes
#include <vector>


namespace GLPL {
    class ILine2D_OLD {
    public:
        ILine2D_OLD();

        void createAndSetupBuffers(GLuint *VAOPt, GLuint *VBOPt, int dataSizeBytes, const void *dataAddress,
                                   int strideBytes, int glType=GL_FLOAT);

        void drawData(Shader shader, glm::mat4 axesLimitViewportTrans, GLuint *VAOPt, glm::vec3 colour, int nPts,
                      GLenum mode);
        void setLineColour(glm::vec3 lineColor);
        void setMode(GLenum newMode);
        void setLineWidth(unsigned int newLineWidth);
        glm::vec3 getColour();
        GLenum getMode();

        virtual void Draw(Shader shader, glm::mat4 axesLimitViewportTrans) = 0;

        virtual std::vector<float> getMinMax() = 0;

    protected:
        /* Attributes */
        unsigned int lineWidth = 1;
        glm::vec3 colour = LC_WHITE;
        GLenum mode; // Mode, line or points
    };
}


#endif //OPENGLPLOTLIVE_ILINE2D_H
