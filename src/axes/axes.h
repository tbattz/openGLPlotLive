//
// Created by tbatt on 19/04/2020.
//

#ifndef OPENGLPLOTLIVE_PROJ_AXES_H
#define OPENGLPLOTLIVE_PROJ_AXES_H

// Project Includes
#include "../texts/TextString.h"
#include "AxesArea.h"
#include "../rendering/ConstantXYDrawable.h"
#include "AxesLineTicks.h"
#include "../scatterPlot/IScatterPlot.h"


namespace GLPL {

    enum AxesType {
        AXES_2D,
        AXES_3D
    };

    class Axes : public ConstantXYDrawable {
    public:
        // Constructor
        Axes(float x, float y, float width, float height, std::shared_ptr<ParentDimensions> parentDimensions);

        // Axes
        void setAxesBoxOn(bool axesBoxOnBool);
        void setLogScale(bool logOn, unsigned int newLogBase, LogAxes logAxes);

        // PressButton
        void setButtonState(const std::string& buttonName, bool activeState);

        void Draw();
        std::string getID();

    protected:
        // Functions
        void createAxesArea();

        // Data
        // Axes Area
        std::shared_ptr<AxesArea> axesArea;

    };
}


#endif //OPENGLPLOTLIVE_PROJ_AXES_H
