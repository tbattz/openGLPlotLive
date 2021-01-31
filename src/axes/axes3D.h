//
// Created by bcub3d-desktop on 30/1/21.
//

#ifndef OPENGLPLOTLIVE_PROJ_AXES3D_H
#define OPENGLPLOTLIVE_PROJ_AXES3D_H

// Project Includes
#include "axes.h"


namespace GLPL {


    class Axes3D : public Axes {
    public:
        // Constructor
        Axes3D(float x, float y, float width, float height, std::shared_ptr<ParentDimensions> parentDimensions);

        // Functions
        // Text
        void addText(const char* textString, float x, float y, float fontSize, AttachLocation attachLocation=BOTTOM_LEFT);
        std::shared_ptr<TextString> getText(unsigned int textStringId);
        void removeTextString(unsigned int textStringId);
        // Line
        std::shared_ptr<ILine2D> addLine(std::vector<float> *dataPtX, std::vector<float> *dataPtY, LineType lineType = SINGLE_LINE,
                                         glm::vec3 colour = LC_WHITE, float opacityRatio=1.0);
        // Scatter Plots
        std::shared_ptr<IScatterPlot> addScatterPlot(std::vector<float> *dataPtX, std::vector<float> *dataPtY,
                                                     glm::vec3 colour = LC_WHITE, float opacityRatio=1.0, MarkerType markerType=CIRCLE);

        // Labels
        void setTitle(std::string newTitle);
        void setXLabel(std::string newXLabel);
        void setYLabel(std::string newYLabel);
        void setXLabelRotation(TextRotation newTextRotation);
        void setYLabelRotation(TextRotation newTextRotation);

    private:
        // Functions
        void createAxesLines();

        // Data
        // Text String
        unsigned int textStringCount = 0;
        std::unordered_map<unsigned int, std::shared_ptr<TextString>> textStringMap;

    };

}


#endif //OPENGLPLOTLIVE_PROJ_AXES3D_H
