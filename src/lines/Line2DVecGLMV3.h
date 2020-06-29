//
// Created by bcub3d-desktop on 29/3/20.
//

#ifndef OPENGLPLOTLIVE_LINE2DVECGLMV3_H
#define OPENGLPLOTLIVE_LINE2DVECGLMV3_H

#include "ILine2D.h"

namespace GLPL {
    class Line2DVecGLMV3 : public  ILine2D {
        // Line of vector of glm::dvec3s
    public:
        /* Constructor */
        Line2DVecGLMV3(std::vector<glm::dvec3>* dataVecPt, int indexX = 0, int indexY = 1, GLenum mode = GL_LINE_STRIP);

        /* Functions */
        void updateInternalData();
        void Draw(Shader shader, glm::mat4 axesLimitViewportTrans);
        std::vector<float> getMinMax();

    private:
        /* Buffers */
        GLuint VAO, VBO;
        int nPts = 0;

        /* Data */
        std::vector<glm::dvec3>* dataVecPt;
        std::vector<float> internalData;

        /* Selection */
        int indexX;
        int indexY;

    };

}


#endif //OPENGLPLOTLIVE_LINE2DVECGLMV3_H
