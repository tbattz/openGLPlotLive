//
// Created by bcub3d-desktop on 2/7/20.
//

#include "AxesLineWithText.h"


namespace GLPL {

    AxesLineWithText::AxesLineWithText(float x, float y, AxesDirection axesDirection, std::shared_ptr<ParentDimensions> parentDimensions) :
        ConstantXYDrawable(x, y, 0.1f, 20.0f, CONSTANT_SCALE, CONSTANT_SIZE, std::move(parentDimensions)) {
        // Set Bounding Box Color
        boundingBoxColor = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);

        // Set Axes Direction
        this->axesDirection = axesDirection;

        // Set Sized Based On Axes Direction
        // TODO
        this->setSize(1.0f, 20.0f);

        // Create Parent Pointers
        std::shared_ptr<ParentDimensions> ourParentDimensions = createParentDimensions();

        // Generate Axes Line Ticks
        axesLineTicks = std::make_shared<AxesLineTicks>(0.0f, 0.0f, axesDirection, ourParentDimensions);


        
    }

    void AxesLineWithText::Draw() {
        // Draw Axes Line
        axesLineTicks->Draw();

        // Draw Axes text
    }

    void AxesLineWithText::setAxesDirection(AxesDirection newAxesDirection) {
        this->axesDirection = newAxesDirection;


    }



}

