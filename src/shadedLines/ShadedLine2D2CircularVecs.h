//
// Created by tbatt on 12/04/2020.
//

#ifndef OPENGLPLOTLIVE_PROJ_SHADEDLINE2D2CIRCULARVECS_H
#define OPENGLPLOTLIVE_PROJ_SHADEDLINE2D2CIRCULARVECS_H

#include "IShadedLine2D.h"


namespace GLPL {
    class ShadedLine2D2CircularVecs : public IShadedLine2D {
    // Two vectors corresponding to an x vector and a y vector
        // An index specifies the 'start' of the vector
        // The resultant vector is from the index to the end of the vector, plus
        // the start to the index.
        // This operates similar to a circular buffer.
    public:
        /* Constructor */
        ShadedLine2D2CircularVecs(std::vector<float>* dataPtX, std::vector<float>* dataPtY, GLenum mode = GL_TRIANGLES);

        /* Destructor */
        ~ShadedLine2D2CircularVecs();

        /* Functions */
        void updateInternalData(unsigned int currIndex);
        void Draw(GLPL::Shader shader, glm::mat4 axesLimitViewportTrans, float zDepth);
        std::vector<float> getMinMax();

    private:
        /* Buffers */
        GLuint VAO, VBO, EBO;
        int nIndices = 0;

        /* Data */
        bool updated = false;
        std::vector<float>* dataPtX;
        std::vector<float>* dataPtY;
        std::vector<float> internalData;
        std::vector<int> internalIndices;

    };


}

#endif //OPENGLPLOTLIVE_PROJ_SHADEDLINE2D2CIRCULARVECS_H
