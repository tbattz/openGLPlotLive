//
// Created by tbatt on 11/04/2020.
//

#ifndef OPENGLPLOTLIVE_PROJ_LINE2D2CIRCULARVECSPOSNEG_H
#define OPENGLPLOTLIVE_PROJ_LINE2D2CIRCULARVECSPOSNEG_H

#include "Line2D2CircularVecs.h"


namespace GLPL  {
    class Line2D2CircularVecsPosNeg : public Line2D2CircularVecs {
        // Alters Line2D2CircularVecs to plot parts of the line that are above and below the line in different colours
    public:
        /* Constructor */
        Line2D2CircularVecsPosNeg(std::vector<float>* dataPtX, std::vector<float>* dataPtY,
                                  glm::vec4 posColor, glm::vec4 negColor, GLenum mode = GL_LINE_STRIP);

        /* Destructor */
        ~Line2D2CircularVecsPosNeg();

        /* Functions */
        void Draw(GLPL::Shader shader, glm::mat4 axesLimitViewportTrans);

    private:
        // Data
        glm::vec4 colPos;
        glm::vec4 colNeg;
    };

}


#endif //OPENGLPLOTLIVE_PROJ_LINE2D2CIRCULARVECSPOSNEG_H
