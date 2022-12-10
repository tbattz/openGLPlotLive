//
// Created by bcub3d-desktop on 26/12/20.
//

#ifndef OPENGLPLOTLIVE_PROJ_ISCATTERPLOT_H
#define OPENGLPLOTLIVE_PROJ_ISCATTERPLOT_H


// Standard Includes
#include <memory>

// Project Includes
#include "../axes/Plotable.h"
#include "../lines/lineColours.h"


namespace GLPL {

    enum MarkerType {
        MARKER_SQUARE,
        MARKER_CIRCLE,
        MARKER_DIAMOND,
        MARKER_TRIANGLE_DOWN,
        MARKER_TRIANGLE_UP,
        MARKER_TRIANGLE_LEFT,
        MARKER_TRIANGLE_RIGHT
    };

    class IScatterPlot : public Plotable {
    public:

        IScatterPlot(std::shared_ptr<ParentDimensions> parentDimensions);

        void setMarkerColour(glm::vec3 newMarkerColour);
        void setOpacityRatio(float newOpacityRatio);
        void setMarkerOutlineColour(glm::vec3 newMarkerOutlineColour);
        void setOutlineOpacityRatio(float newOpacityRatio);
        void setMarkerSizePx(float newMarkerSizePx);
        void setMarkerType(MarkerType newMarkerType);
        glm::vec3 getColour();
        int getHoverCursor() override;

        virtual std::vector<float> getMinMax(bool onlyPositiveX, bool onlyPositiveY) = 0;
        virtual void generateAllMarkerVerts() = 0;

    protected:
        // Marker Properties
        glm::vec3 markerColour = LC_WHITE;
        glm::vec3 markerOutlineColour = LC_WHITE;
        float opacityRatio = 0.7;
        float outlineOpacityRatio = 1.0;
        float markerSizePx = 15; // Width/Height of the marker in pixels
        MarkerType markerType = MARKER_SQUARE;

    };

}


#endif //OPENGLPLOTLIVE_PROJ_ISCATTERPLOT_H
