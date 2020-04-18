//
// Created by bcub3d-desktop on 29/3/20.
//

#ifndef OPENGLPLOTLIVE_LINE2DVEC_H
#define OPENGLPLOTLIVE_LINE2DVEC_H


#include "../dataTypes/dataTypes.h"
#include "ILine2D.h"



namespace GLPL {
    // Class template
    template <typename T>
    class Line2DVec : public ILine2D {
        // Line of 2-length vectors
    public:
        /* Constructor */
        Line2DVec(std::vector<T>* dataVecPt, GLenum mode = GL_LINE_STRIP);

        /* Functions */
        void Draw(Shader shader, glm::mat4 axesLimitViewportTrans);
        void appendVec(T x, T y);
        std::vector<float> getMinMax();

    private:
        /* Buffers */
        GLuint VAO, VBO;
        int nPts = 0;

        /* Data */
        std::vector<T>* dataVecPt;
    };
}


#endif //OPENGLPLOTLIVE_LINE2DVEC_H
