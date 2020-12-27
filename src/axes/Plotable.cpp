//
// Created by tbatt on 24/06/2020.
//

#include "Plotable.h"

#include <utility>


namespace GLPL {

    Plotable::Plotable(std::shared_ptr<ParentDimensions> parentDimensions) :
        ConstantXYDrawable(0.0f, 0.0f, 1.0f, 1.0f, CONSTANT_SCALE, CONSTANT_SCALE, std::move(parentDimensions)) {

    }

    void Plotable::setAxesViewportTransform(std::shared_ptr<glm::mat4> newAxesViewportTransform) {
        this->axesViewportTransform = std::move(newAxesViewportTransform);
    }

    void Plotable::setPlotableId(int newPlotableId) {
        plotableId = newPlotableId;
    }


}