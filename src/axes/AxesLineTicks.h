//
// Created by bcub3d-desktop on 4/7/20.
//

#ifndef OPENGLPLOTLIVE_PROJ_AXESLINETICKS_H
#define OPENGLPLOTLIVE_PROJ_AXESLINETICKS_H


// Project Includes
#include "../rendering/IDrawable.h"
#include "../rendering/ConstantXYDrawable.h"


namespace GLPL {

    enum AxesDirection {
        X_AXES_TOP,     // Attached to top, line on bottom
        X_AXES_BOTTOM,  // Attached to bottom, line on top
        X_AXES_CENTRE,  // Attached at centre, line in centre
        Y_AXES_LEFT,    // Attached to left, line on right
        Y_AXES_RIGHT,   // Attached to right, line on left
        Y_AXES_CENTRE   // Attached to centre, line on right
    };

    class AxesLineTicks : public ConstantXYDrawable {
    public:
        // Constructor
        AxesLineTicks(AxesDirection axesDirection, std::shared_ptr<ParentDimensions> parentDimensions);

        // Functions
        void Draw();

    private:
        // Functions
        void generateAxesLine();
        void updateSize();
        void setMinMax(float newMin, float newMax);
        void createAndSetupAxesLineBuffer();
        void drawAxesLine();

        // Buffers
        AxesDirection axesDirection;
        GLuint lineVAO, lineVBO;
        std::vector<GLfloat> lineVerts;
        glm::vec4 axesLineColor = glm::vec4(0.75f, 0.75f, 0.75f, 1.0f);

        // Limits
        float valMin = -1;
        float valMax = 1;
        unsigned int minorTickSpacingPx = 25;   // Space between minor ticks
        unsigned int minorTickLengthPx = 10;    // Length of minor ticks
        unsigned int majorTickLengthPx = 20;    // Length of major ticks
        unsigned int minorSpacingsPerMajor = 3; // Number of minor spacings between two major spacings

    };

}


#endif //OPENGLPLOTLIVE_PROJ_AXESLINETICKS_H
