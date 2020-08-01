//
// Created by bcub3d-desktop on 2/7/20.
//

#ifndef OPENGLPLOTLIVE_PROJ_AXESLINEWITHTEXT_H
#define OPENGLPLOTLIVE_PROJ_AXESLINEWITHTEXT_H

// Project Includes
#include "../texts/textString.h"
#include "AxesLineTicks.h"
#include "../rendering/ConstantXYDrawable.h"


namespace GLPL {



    class AxesLineWithText : public ConstantXYDrawable {
    public:
        // Constructor
        AxesLineWithText(float x, float y, AxesDirection axesDirection, std::shared_ptr<ParentDimensions> parentDimensions);

        // Functions
        void Draw();
        void setAxesDirection(AxesDirection newAxesDirection);
        //void setMinorTickLength(unsigned int tickLengthPx);
        //void setMajorTickLength(unsigned int tickLengthPx);


    private:
        // Axes Line
        std::shared_ptr<AxesLineTicks> axesLineTicks;

        // Buffers
        AxesDirection axesDirection;



        // Axes Text
        unsigned int textFontSize = 10;
        float spacingFactor = 1.1;
        std::vector<TextString> axesText;

    };
}


#endif //OPENGLPLOTLIVE_PROJ_AXESLINEWITHTEXT_H
