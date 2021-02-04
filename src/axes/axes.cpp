//
// Created by tbatt on 19/04/2020.
//

// Standard Includes
#include <utility>

// Project Includes
#include "axes.h"



namespace GLPL {

    Axes::Axes(float x, float y, float width, float height,
               std::shared_ptr<ParentDimensions> parentDimensions) :
            ConstantXYDrawable(x, y, width, height, CONSTANT_SCALE, CONSTANT_SCALE, std::move(parentDimensions)) {

        // Set Bounding Box Color
        boundingBoxColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
        // Set Not Hoverable
        setHoverable(false);

        // Add axes area
        Axes::createAxesArea();

    }

    void Axes::createAxesArea() {
        // Create Parent Dimensions
        std::shared_ptr<ParentDimensions> newParentPointers = IDrawable::createParentDimensions();
        // Register Child
        std::shared_ptr<IDrawable> axesAreaObj = std::make_shared<AxesArea>(0.16, 0.1, 0.75, 0.75, newParentPointers);
        std::shared_ptr<AxesArea> axesAreaPt = std::dynamic_pointer_cast<AxesArea>(axesAreaObj);
        Axes::registerChild(axesAreaObj);
        // Store Text String
        this->axesArea = axesAreaPt;
    }

    void Axes::Draw() {
        // Sort children if required
        IDrawable::sortChildren();
        // Draw children
        for(auto & i : children) {
            i->Draw();
        }
    }

    std::string Axes::getID() {
        return "Axes:" + std::to_string(x) + ":" + std::to_string(y);
    }

    void Axes::setAxesBoxOn(bool axesBoxOnBool) {
        axesArea->setAxesBoxOn(axesBoxOnBool);
    }

    void Axes::setLogScale(bool logOn, unsigned int newLogBase, LogAxes logAxes) {
        axesArea->setLogScale(logOn, newLogBase, logAxes);
    }

    void Axes::setButtonState(const std::string& buttonName, bool activeState) {
        axesArea->setButtonState(buttonName, activeState);
    }

    std::vector<float> Axes::getAxesInfo() {
        return std::vector<float> {getX(), getY(), getWidth(), getHeight()};
    }

}