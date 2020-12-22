//
// Created by tbatt on 24/06/2020.
//

#include "IPlotable.h"

#include <utility>


namespace GLPL {

    IPlotable::IPlotable() {

    }

    void IPlotable::setAxesViewportTransform(std::shared_ptr<glm::mat4> newAxesViewportTransform) {
        this->axesViewportTransform = std::move(newAxesViewportTransform);
    }


}