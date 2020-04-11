//
// Created by bcub3d-desktop on 27/3/20.
//

#ifndef OPENGLPLOTLIVE_LINE2DPTS_H
#define OPENGLPLOTLIVE_LINE2DPTS_H


#include "../dataTypes/dataTypes.h"
#include "ILine2D.h"
#include "lineColours.h"


namespace GLPL {
    class Line2DPts : public ILine2D {
        // Line of pt2D Structures
    public:
        /* Constructor */
        Line2DPts(std::vector<GLPL::pt2D>* dataPtPt, GLenum mode = GL_LINE_STRIP);

        /* Destructor */
        ~Line2DPts();

        /* Functions */
        void Draw(GLPL::Shader shader, glm::mat4 axesLimitViewportTrans);
        std::vector<float> getMinMax();


    private:
        /* Buffers */
        GLuint VAO, VBO;
        int nPts = 0;

        /* Data */
        std::vector<GLPL::pt2D> *dataPtPt;

    };
}

#endif //OPENGLPLOTLIVE_LINE2DPTS_H
