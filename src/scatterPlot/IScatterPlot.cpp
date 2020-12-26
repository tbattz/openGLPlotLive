//
// Created by bcub3d-desktop on 26/12/20.
//

#include "IScatterPlot.h"


namespace GLPL {

    IScatterPlot::IScatterPlot(std::shared_ptr<ParentDimensions> parentDimensions) :
            ConstantXYDrawable(0.0f, 0.0f, 1.0f, 1.0f,
                               CONSTANT_SCALE, CONSTANT_SCALE, std::move(parentDimensions)) {};

    void IScatterPlot::setMarkerColour(glm::vec3 newMarkerColour) {
        markerColour = newMarkerColour;
    }

    void IScatterPlot::setOpacityRatio(float newOpacityRatio) {
        opacityRatio = newOpacityRatio;
    }

    void IScatterPlot::setMarkerOutlineColour(glm::vec3 newMarkerColour) {
        markerOutlineColour = newMarkerColour;
    }

    void IScatterPlot::setOutlineOpacityRatio(float newOpacityRatio) {
        outlineOpacityRatio = newOpacityRatio;
    }

    glm::vec3 IScatterPlot::getColour() {
        return markerColour;
    }

    int IScatterPlot::getHoverCursor() {
        return 0;
    }

}