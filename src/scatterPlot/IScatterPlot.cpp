//
// Created by bcub3d-desktop on 26/12/20.
//

#include "IScatterPlot.h"


namespace GLPL {

    IScatterPlot::IScatterPlot(std::shared_ptr<ParentDimensions> parentDimensions) :
            Plotable(std::move(parentDimensions)) {};

    void IScatterPlot::setMarkerColour(glm::vec3 newMarkerColour) {
        markerColour = newMarkerColour;
    }

    void IScatterPlot::setOpacityRatio(float newOpacityRatio) {
        opacityRatio = newOpacityRatio;
    }

    void IScatterPlot::setMarkerOutlineColour(glm::vec3 newMarkerColour) {
        markerOutlineColour = newMarkerColour;
    }

    void IScatterPlot::setMarkerSizePx(float newMarkerSizePx) {
        markerSizePx = newMarkerSizePx;

        generateAllMarkerVerts();
    }

    void IScatterPlot::setOutlineOpacityRatio(float newOpacityRatio) {
        outlineOpacityRatio = newOpacityRatio;
    }

    void IScatterPlot::setMarkerType(MarkerType newMarkerType) {
        markerType = newMarkerType;

        generateAllMarkerVerts();
    }

    glm::vec3 IScatterPlot::getColour() {
        return markerColour;
    }

    int IScatterPlot::getHoverCursor() {
        return 0;
    }

}