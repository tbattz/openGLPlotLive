//
// Created by tbatt on 19/04/2020.
//

#include "plot2.h"

#include <utility>

namespace GLPL {

    Plot2::Plot2(float x, float y, float width, float height, ParentPointers parentPointers) :
            IDrawable(x, y, width, height, std::move(parentPointers)) {

        // Set Bounding Box Color
        boundingBoxColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

        // Add default axes
        Plot2::addAxes(0.1f, 0.1f, 0.7f, 0.6f);

    }

    Plot2::Plot2::~Plot2() {

    }

    void Plot2::addAxes(float x, float y, float width, float height) {
        // Create axes
        ParentPointers newParentPointers = IDrawable::createParentPointers();
        Axes2 axes2 = Axes2(x, y, width, height, newParentPointers);
        // Register child
        std::shared_ptr<IDrawable> axesPt = std::make_shared<Axes2>(axes2);
        std::shared_ptr<Axes2> axesPt2 = std::make_shared<Axes2>(axes2);
        Plot2::registerChild(axesPt);
        // Store axes
        axesList.push_back(axesPt2);
    }

    void Plot2::Plot2::Draw() {

        for(unsigned int i=0; i < children.size(); i++) {
            children[i]->Draw();
        }
    }

}