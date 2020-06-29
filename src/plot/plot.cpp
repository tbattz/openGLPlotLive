//
// Created by tbatt on 19/04/2020.
//

// Project Includes
#include "plot.h"


namespace GLPL {

    Plot::Plot(float x, float y, float width, float height, std::shared_ptr<ParentDimensions> parentDimensions) :
            ConstantScaleDrawable(x, y, width, height, std::move(parentDimensions)) {

        // Set Bounding Box Color
        boundingBoxColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

        // Add default axes
        Plot::addAxes(0.1f, 0.1f, 0.7f, 0.6f);

    }

    Plot::~Plot() {

    }

    void Plot::addAxes(float x, float y, float width, float height) {
        // Create axes
        std::shared_ptr<ParentDimensions> newParentPointers = IDrawable::createParentDimensions();
        // Register child
        std::shared_ptr<IDrawable> axes = std::make_shared<Axes>(x, y, width, height, newParentPointers);
        std::shared_ptr<Axes> axesPt = std::dynamic_pointer_cast<Axes>(axes);
        Plot::registerChild(axes);
        // Store axes
        axesMap.insert(std::pair<unsigned int, std::shared_ptr<Axes>>(axesCount, axesPt));
        axesCount += 1;
    }

    std::shared_ptr<Axes> Plot::getAxes(unsigned int axesId) {
        if (axesMap.count(axesId) > 0) {
            return axesMap.at(axesId);
        } else {
            std::cout << "Axes " << axesId << " does not exist!" << std::endl;
            return nullptr;
        }
    }

    void Plot::removeAxes(unsigned int axesId) {
        if (axesMap.count(axesId) > 0) {
            std::shared_ptr<Axes> axes2Remove = axesMap.at(axesId);
            // Remove child
            IDrawable::removeChild(axes2Remove);
            // Remove axes
            axesMap.erase(axesId);
        } else {
            std::cout << "Cannot remove Axes " << axesId << ", axes does not exist!" << std::endl;
        }
    }

    void Plot::Draw() {
        // Sort children if required
        IDrawable::sortChildren();
        // Draw children
        for(auto & i : children) {
            i->Draw();
        }
    }

}