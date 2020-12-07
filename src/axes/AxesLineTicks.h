//
// Created by bcub3d-desktop on 4/7/20.
//

#ifndef OPENGLPLOTLIVE_PROJ_AXESLINETICKS_H
#define OPENGLPLOTLIVE_PROJ_AXESLINETICKS_H


// Standard Includes
#include <math.h>

// Project Includes
#include "../rendering/IDrawable.h"
#include "../rendering/ConstantXYDrawable.h"
#include "../texts/textString.h"


namespace GLPL {

    enum AxesDirection {
        X_AXES_TOP,     // Attached to top, line on bottom
        X_AXES_BOTTOM,  // Attached to bottom, line on top
        X_AXES_CENTRE,  // Attached at centre, line in centre
        Y_AXES_LEFT,    // Attached to left, line on right
        Y_AXES_RIGHT,   // Attached to right, line on left
        Y_AXES_CENTRE   // Attached to centre, line on right
    };

    enum TickSpacingType {
        MINOR_SPACING,
        MAJOR_SPACING
    };

    class AxesLineTicks : public ConstantXYDrawable {
    public:
        // Constructor
        AxesLineTicks(AxesDirection axesDirection, std::shared_ptr<ParentDimensions> parentDimensions);

        // Functions
        void Draw();
        std::string getID();
        void setParentDimensions(glm::mat4 newParentTransform,
                                 int newParentXPx,
                                 int newParentYPx,
                                 int newParentWidthPx,
                                 int newParentHeightPx);
        void setParentDimensions(std::shared_ptr<ParentDimensions> parentDimensions);
        void setMinMax(float newXMin, float newXMax, float newYMin, float newYMax);


    private:
        // Functions
        void generateAllVertices();
        // Axes Line
        void generateAxesLines();
        // Axes Ticks
        std::tuple<unsigned int, std::vector<TickSpacingType>, std::vector<float>, std::vector<float>>
                                generateEquallySpacingBetweenLimits(float sectionWidthRel,
                                                                    float sectionWidthAxesUnits,
                                                                    float midRelPos);
        void generateXTopTickVerts(TickSpacingType tickSpacingType, float relPos, float axesPos);
        void generateXBottomTickVerts(TickSpacingType tickSpacingType, float relPos, float axesPos);
        void generateXCentreTickVerts(TickSpacingType tickSpacingType, float relPos, float axesPos);
        void generateYLeftTickVerts(TickSpacingType tickSpacingType, float relPos, float axesPos);
        void generateYRightTickVerts(TickSpacingType tickSpacingType, float relPos, float axesPos);
        void generateYCentreTickVerts(TickSpacingType tickSpacingType, float relPos, float axesPos);
        void generateXTickVerts();
        void generateYTickVerts();
        void generateTickVerts();
        std::pair<float, float>  generateTickLabelVerts(float xPos, float yPos);
        // Axes Tick Labels
        AttachLocation generateMajorTickOffsetAttachLocation();
        std::string value2Str(float inValue, unsigned int maxChar = 3, unsigned int maxDecimal = 2);
        void generateMajorTickLabels();
        // Pixel DPI Scaling
        void setPixelSpacing(unsigned int newMinorTickSpacingPx, unsigned int newMinorTickLengthPx, unsigned int newMajorTickLengthPx);

        void updateSize();
        void createAndSetupAxesLineBuffers();
        void updateAxesLineBuffers();
        void drawAxesLine();
        void drawMajorTicks();
        void drawMinorTicks();
        void drawMajorTickLabels();

        // Axes Line Buffers
        AxesDirection axesDirection;
        GLuint axesLineVAO, axesLineVBO;
        std::vector<GLfloat> axesLineVerts;

        // Major Tick Buffers
        GLuint majorTickVAO, majorTickVBO;
        std::vector<GLfloat> majorTickVerts;    // Holds the vertices (-1 to 1) to plot
        std::vector<float> majorTickAxesPos;    // Holds the graph value at this vertex

        // Minor Tick Buffers
        GLuint minorTickVAO, minorTickVBO;
        std::vector<GLfloat> minorTickVerts;    // Holds the vertices (-1 to 1) to plot
        std::vector<float> minorTickAxesPos;    // Holds the graph value at this vertex

        // Axes Text Labels
        float xOffsetFactor = 1.2f;
        float yOffsetFactor = 1.2f;
        std::vector<std::shared_ptr<TextString>> majorTickTextStrings; // Holds the text labels for major axes ticks

        // Settings
        float xMin = -1;
        float xMax = 1;
        float yMin = -1;
        float yMax = 1;
        glm::vec4 axesLineColor = glm::vec4(0.75f, 0.75f, 0.75f, 1.0f);
        unsigned int minorTickSpacingPx = 20;   // Suggested space between minor ticks
        unsigned int minorTickLengthPx = 10;    // Length of minor ticks
        unsigned int majorTickLengthPx = 20;    // Length of major ticks
        unsigned int minorSpacingsPerMajor = 3; // Number of minor spacings between two major spacings

    };

}


#endif //OPENGLPLOTLIVE_PROJ_AXESLINETICKS_H
