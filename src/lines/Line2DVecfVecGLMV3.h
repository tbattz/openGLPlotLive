//
// Created by bcub3d-desktop on 29/3/20.
//

#ifndef OPENGLPLOTLIVE_LINE2DVECFVECGLMV3_H
#define OPENGLPLOTLIVE_LINE2DVECFVECGLMV3_H

#include "../dataTypes/dataTypes.h"
#include "ILine2D_OLD.h"


namespace GLPL {
    class Line2DVecfVecGLMV3 : public ILine2D_OLD {
        // Line of vector<float> vs vector of glm::dvec3s
        // For plotting a vector of floats against an index selected form a vector of glm::dvec3s
        // For example plotting a vector of time data against a vector of glm::dvec3.

    public:
        /* Constructor */
        Line2DVecfVecGLMV3(std::vector<float>* dataVecfPt, std::vector<glm::dvec3>* dataVecGLMV3Pt, int indexGLM = 0, GLenum mode = GL_LINE_STRIP);

        /* Functions */
        void updateInternalData();
        void Draw(Shader shader, glm::mat4 axesLimitViewportTrans);
        std::vector<float> getMinMax();

    private:
        /* Buffers */
        GLuint VAO, VBO;
        int nPts = 0;

        /* Data */
        std::vector<float>*	dataVecfPt;
        std::vector<glm::dvec3>* dataVecGLMV3Pt;
        std::vector<float> internalData;

        /* Selection */
        int indexGLM;

    };
}


#endif //OPENGLPLOTLIVE_LINE2DVECFVECGLMV3_H
