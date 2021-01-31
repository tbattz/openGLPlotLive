//
// Created by tbatt on 19/04/2020.
//

// Project Includes
#include "plot.h"
#include "../axes/axes2D.h"
#include "../axes/axes3D.h"


namespace GLPL {

    Plot::Plot(float x, float y, float width, float height, std::shared_ptr<ParentDimensions> parentDimensions) :
            ConstantXYDrawable(x, y, width, height, CONSTANT_SCALE, CONSTANT_SCALE, std::move(parentDimensions)) {

        // Set Bounding Box Color
        boundingBoxColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

        // Add default axes
        Plot::addAxes(0.0f, 0.0f, 0.5f, 1.0f, AXES_2D);

    }

    Plot::~Plot() {

    }

    std::shared_ptr<GLPL::Axes2D> Plot::add2DAxes(float x, float y, float width, float height) {
        return std::dynamic_pointer_cast<Axes2D>(Plot::addAxes(x, y, width, height, AXES_2D));
    }

    std::shared_ptr<GLPL::Axes3D> Plot::add3DAxes(float x, float y, float width, float height) {
        return std::dynamic_pointer_cast<Axes3D>(Plot::addAxes(x, y, width, height, AXES_3D));
    }

    std::shared_ptr<GLPL::Axes> Plot::addAxes(float x, float y, float width, float height, AxesType axesType) {
        // Create parent dimensions
        std::shared_ptr<ParentDimensions> newParentPointers = IDrawable::createParentDimensions();
        // Create axes
        std::shared_ptr<IDrawable> axes = nullptr;
        switch(axesType) {
            case AXES_2D: {
                axes = std::make_shared<Axes2D>(x, y, width, height, newParentPointers);
                break;
            }
            case AXES_3D: {
                axes = std::make_shared<Axes3D>(x, y, width, height, newParentPointers);
                break;
            }
            default: {
                std::cout << "Unknown AxesType! - " << axesType << std::endl;
            }
        }
        std::shared_ptr<Axes> axesPt = std::dynamic_pointer_cast<Axes>(axes);
        // Register child
        Plot::registerChild(axes);
        // Store axes
        axesMap.insert(std::pair<unsigned int, std::shared_ptr<Axes>>(axesCount, axesPt));
        axesCount += 1;

        return axesPt;
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

    std::string Plot::getID() {
        return "Plot:" + std::to_string(x) + ":" + std::to_string(y);
    }

}