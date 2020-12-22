//
// Created by bcub3d-desktop on 29/3/20.
//

#ifndef OPENGLPLOTLIVE_LINE2DVEC_H
#define OPENGLPLOTLIVE_LINE2DVEC_H


#include "../dataTypes/dataTypes.h"
#include "ILine2D_OLD.h"



namespace GLPL {
    // Template the vector type
    // Set mappings from Type T to GL Types
    template <class T> constexpr int getGLType();

    template <> constexpr int getGLType<int>() { return GL_INT; }
    template <> constexpr int getGLType<float>() { return GL_FLOAT; }
    template <> constexpr int getGLType<double>() { return GL_DOUBLE; }

    // Class template
    template <typename T>
    class Line2DVec : public ILine2D_OLD {
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
