//
// Created by tbatt on 11/04/2020.
//

#ifndef OPENGLPLOTLIVE_PROJ_LINE2D2CIRCULARVECS_H
#define OPENGLPLOTLIVE_PROJ_LINE2D2CIRCULARVECS_H

#include "ILine2D_OLD.h"


namespace GLPL  {
    class Line2D2CircularVecs : public ILine2D_OLD {
        // Two vectors corresponding to an x vector and a y vector
        // An index specifies the 'start' of the vector
        // The resultant vector is from the index to the end of the vector, plus
        // the start to the index.
        // This operates similar to a circular buffer.
    public:
        /* Constructor */
        Line2D2CircularVecs(std::vector<float>* dataPtX, std::vector<float>* dataPtY, GLenum mode = GL_LINE_STRIP);

        /* Destructor */
        ~Line2D2CircularVecs();

        /* Functions */
        void updateInternalData(unsigned int currIndex);
        void Draw(GLPL::Shader shader, glm::mat4 axesLimitViewportTrans);
        std::vector<float> getMinMax();

    protected:
        /* Buffers */
        GLuint VAO, VBO;
        int nPts = 0;

        /* Data */
        bool updated = false;
        std::vector<float>* dataPtX;
        std::vector<float>* dataPtY;
        std::vector<float> internalData;

    };

}


#endif //OPENGLPLOTLIVE_PROJ_LINE2D2CIRCULARVECS_H
