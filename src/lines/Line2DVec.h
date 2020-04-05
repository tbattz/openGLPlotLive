//
// Created by bcub3d-desktop on 29/3/20.
//

#ifndef OPENGLPLOTLIVE_LINE2DVEC_H
#define OPENGLPLOTLIVE_LINE2DVEC_H


#include "../dataTypes/dataTypes.h"
#include "ILine2D.h"



namespace GLPL {
    class Line2DVec : public ILine2D {
        // Line of 2-length vectors
    public:
        /* Constructor */
        Line2DVec(std::vector<float>* dataVecPt, GLenum mode = GL_LINE_STRIP);

        /* Functions */
        void Draw(Shader shader, glm::mat4 axesLimitViewportTrans);
        void appendVec(float x, float y);
        std::vector<float> getMinMax();

    private:
        /* Buffers */
        GLuint VAO, VBO;
        int nPts = 0;

        /* Data */
        std::vector<float>* dataVecPt;
    };
}


#endif //OPENGLPLOTLIVE_LINE2DVEC_H
