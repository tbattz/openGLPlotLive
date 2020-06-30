//
// Created by bcub3d-desktop on 29/3/20.
//

#ifndef OPENGLPLOTLIVE_LINE2DVECVEC_H
#define OPENGLPLOTLIVE_LINE2DVECVEC_H


#include "../dataTypes/dataTypes.h"
#include "ILine2D_OLD.h"


namespace GLPL {
    class Line2DVecVec : public ILine2D_OLD {
        // Line of vectors of vectors
    public:
        /* Constructor */
        Line2DVecVec(std::vector<std::vector<float>>* dataVecPt, int indexX = 0, int indexY = 1, GLenum mode = GL_LINE_STRIP);

        /* Functions */
        void updateInternalData();
        void Draw(Shader shader, glm::mat4 axesLimitViewportTrans);
        std::vector<float> getMinMax();

    private:
        /* Buffers */
        GLuint VAO, VBO;
        int nPts = 0;

        /* Data */
        std::vector<std::vector<float>>* dataVecPt;
        std::vector<float> internalData;

        /* Selection */
        int indexX;
        int indexY;

    };
}


#endif //OPENGLPLOTLIVE_LINE2DVECVEC_H
