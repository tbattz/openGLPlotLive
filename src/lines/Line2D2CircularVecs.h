//
// Created by tbatt on 11/04/2020.
//

#ifndef OPENGLPLOTLIVE_PROJ_LINE2D2CIRCULARVECS_H
#define OPENGLPLOTLIVE_PROJ_LINE2D2CIRCULARVECS_H

#include "../dataTypes/dataTypes.h"
#include "ILine2D.h"


namespace GLPL  {
    // Class template
    template <typename T, typename U>
    class Line2D2CircularVecs : public ILine2D {
        // Two vectors corresponding to an x vector and a y vector
        // An index specifies the 'start' of the vector
        // The resultant vector is from the index to the end of the vector, plus
        // the start to the index.
        // This operates similar to a circular buffer.
    public:
        /* Constructor */
        Line2D2CircularVecs(std::vector<T>* dataPtX, std::vector<U>* dataPtY, GLenum mode = GL_LINE_STRIP);

        /* Functions */
        void updateInternalData(unsigned int currIndex);
        void Draw(GLPL::Shader shader, glm::mat4 axesLimitViewportTrans);
        std::vector<float> getMinMax();

    protected:
        /* Buffers */
        GLuint VAO, VBO;
        int nPts = 0;

        // Pick the larger of the two types to cast to
        using largerType = typename std::conditional<sizeof(T) >= sizeof(U), T, U>::type;

        /* Data */
        bool updated = false;
        std::vector<T>* dataPtX;
        std::vector<U>* dataPtY;
        std::vector<largerType> internalData;

    };

    // Force the compiler to generate templates of these types - fixes undefined reference error
    template class Line2D2CircularVecs<int, int>;
    template class Line2D2CircularVecs<int, float>;
    template class Line2D2CircularVecs<int, double>;
    template class Line2D2CircularVecs<float, int>;
    template class Line2D2CircularVecs<float, float>;
    template class Line2D2CircularVecs<float, double>;
    template class Line2D2CircularVecs<double, int>;
    template class Line2D2CircularVecs<double, float>;
    template class Line2D2CircularVecs<double, double>;

}


#endif //OPENGLPLOTLIVE_PROJ_LINE2D2CIRCULARVECS_H
