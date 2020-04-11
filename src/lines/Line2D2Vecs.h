//
// Created by tbatt on 11/04/2020.
//

#ifndef OPENGLPLOTLIVE_PROJ_LINE2D2VECS_H
#define OPENGLPLOTLIVE_PROJ_LINE2D2VECS_H

#include "ILine2D.h"


namespace GLPL  {
    class Line2D2Vecs : public ILine2D {
        // Two vectors corresponding to an x vector and a y vector
    public:
        /* Constructor */
        Line2D2Vecs(std::vector<float>* dataPtX, std::vector<float>* dataPtY, GLenum mode = GL_LINE_STRIP);

        /* Destructor */
        ~Line2D2Vecs();

        /* Functions */
        void updateInternalData();
        void Draw(GLPL::Shader shader, glm::mat4 axesLimitViewportTrans);
        std::vector<float> getMinMax();

    private:
        /* Buffers */
        GLuint VAO, VBO;
        int nPts = 0;

        /* Data */
        std::vector<float>* dataPtX;
        std::vector<float>* dataPtY;
        std::vector<float> internalData;

    };

}


#endif //OPENGLPLOTLIVE_PROJ_LINE2D2VECS_H
