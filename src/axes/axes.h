//
// Created by tbatt on 19/04/2020.
//

#ifndef OPENGLPLOTLIVE_PROJ_AXES_H
#define OPENGLPLOTLIVE_PROJ_AXES_H

// Project Includes
#include "../texts/textString.h"
#include "AxesArea.h"
#include "../rendering/ConstantXYDrawable.h"
#include "AxesLineTicks.h"


namespace GLPL {
    class Axes : public ConstantXYDrawable {
    public:
        // Constructor
        Axes(float x, float y, float width, float height, std::shared_ptr<ParentDimensions> parentDimensions);

        // Functions
        // Text
        void addText(const char* textString, float x, float y, float fontSize, AttachLocation attachLocation=BOTTOM_LEFT);
        std::shared_ptr<TextString> getText(unsigned int textStringId);
        void removeTextString(unsigned int textStringId);
        // Line
        std::shared_ptr<ILine2D> addLine(std::vector<float> *dataPtX, std::vector<float> *dataPtY, LineType lineType = SINGLE_LINE,
                     glm::vec3 colour = LC_WHITE, float opacityRatio=1.0);

        void Draw();

    private:
        // Functions
        void createAxesArea();
        void createAxesLines();

        // Data
        // Axes Area
        std::shared_ptr<AxesArea> axesArea;
        // Text String
        unsigned int textStringCount = 0;
        std::unordered_map<unsigned int, std::shared_ptr<TextString>> textStringMap;
        // Axes
        std::shared_ptr<AxesLineTicks> xAxes;
        std::shared_ptr<AxesLineTicks> yAxes;
    };
}


#endif //OPENGLPLOTLIVE_PROJ_AXES_H
